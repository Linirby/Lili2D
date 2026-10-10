#include "lili2d/render/2d/dynamic_batch.hpp"

#include <memory>

#include "lili2d/geometry/mat3x3.hpp"
#include "lili2d/geometry/vec2.hpp"
#include "lili2d/render/2d/batch_helper.hpp"
#include "lili2d/render/common/material.hpp"
#include "lili2d/render/common/model.hpp"
#include "lili2d/render/gpu/gpu_mesh.hpp"
#include "lili2d/render/gpu/pass_types.hpp"
#include "lili2d/render/gpu/texture.hpp"
#include "lili2d/render/renderer.hpp"

namespace lili {

DynamicBatch::DynamicBatch(Renderer* renderer, Texture* texture)
  : renderer(renderer)
{
    batch_pool.push_back(
        BatchItem{
            .mesh = std::make_unique<GPUMesh>(renderer->getDevice(), mesh_data),
            .material = Material(texture) }
    );
    active_texture = texture;
}

void
DynamicBatch::addSprite(
    const SliceUV& slice,
    Vec2 position,
    Vec2 scale,
    float rotation,
    Vec4 tint
)
{
    BatchHelper::appendSprite(
        mesh_data, slice, position, scale, rotation, tint
    );
    if (active_texture != slice.texture)
        active_texture = slice.texture;
}

void
DynamicBatch::addRect(
    Vec2 position,
    Vec2 size,
    Vec2 scale,
    float rotation,
    Vec4 color,
    bool hollow,
    float thickness
)
{
    if (hollow)
        BatchHelper::appendRect(
            mesh_data, position, size, scale, rotation, color
        );
    else
        BatchHelper::appendHollowRect(
            mesh_data, position, size, scale, rotation, color, thickness
        );
    Texture* the_white_pixel = renderer->getTheWhitePixel();
    if (active_texture != the_white_pixel)
        active_texture = the_white_pixel;
}

void
DynamicBatch::flush(float layer, RenderLayer render_pass)
{
    if (mesh_data.vertices.empty())
        return;

    if (pool_idx >= batch_pool.size())
        batch_pool.push_back(
            BatchItem{
                .mesh =
                    std::make_unique<GPUMesh>(renderer->getDevice(), mesh_data),
                .material = Material(active_texture) }
        );
    BatchItem& current_item = batch_pool[pool_idx++];
    current_item.mesh->update(mesh_data, renderer->getCurrentCommandBuffer());
    current_item.material.albedoMap = active_texture;

    renderer->submit(
        Model(current_item.mesh.get(), &current_item.material),
        Mat3::identity(),
        layer,
        render_pass
    );

    mesh_data.vertices.clear();
    mesh_data.indices.clear();
}

} // namespace lili
