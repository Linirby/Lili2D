#include "lili2d/render/2d/static_batch.hpp"
#include "lili2d/render/2d/batch_helper.hpp"

namespace lili {

StaticBatch::StaticBatch(Renderer* renderer, Texture* texture)
  : renderer(renderer)
  , active_texture(texture)
  , material(texture)
{
}

void
StaticBatch::addSprite(
    const SliceUV& slice,
    Vec2 pos,
    Vec2 scale,
    float rotation,
    Vec4 color
)
{
    BatchHelper::appendSprite(mesh_data, slice, pos, scale, rotation, color);
    if (active_texture != slice.texture) {
        active_texture = slice.texture;
        material.albedoMap = active_texture;
    }
}

void
StaticBatch::addRect(
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
        BatchHelper::appendHollowRect(
            mesh_data, position, size, scale, rotation, color, thickness
        );
    else
        BatchHelper::appendRect(
            mesh_data, position, size, scale, rotation, color
        );
    Texture* the_white_pixel = renderer->getTheWhitePixel();
    if (active_texture != the_white_pixel)
        active_texture = the_white_pixel;
}

void
StaticBatch::addCircle(
    Vec2 center_pos,
    float radius,
    float segments,
    Vec4 color,
    bool hollow,
    float thickness
)
{
    if (hollow)
        BatchHelper::appendHollowCircle(
            mesh_data, center_pos, radius, segments, color, thickness
        );
    else
        BatchHelper::appendCircle(
            mesh_data, center_pos, radius, segments, color
        );

    Texture* the_white_pixel = renderer->getTheWhitePixel();
    if (active_texture != the_white_pixel)
        active_texture = the_white_pixel;
}

void
StaticBatch::bake()
{
    if (mesh_data.vertices.empty())
        return;

    if (!mesh)
        mesh = std::make_unique<GPUMesh>(renderer->getDevice(), mesh_data);
    else
        mesh->update(mesh_data, renderer->getCurrentCommandBuffer());
    material.albedoMap = active_texture;

    mesh_data.vertices.clear();
    mesh_data.indices.clear();
}

void
StaticBatch::draw(float layer, RenderLayer render_pass)
{
    if (!mesh_data.vertices.empty())
        bake();

    if (!mesh || mesh->getIndexCount() == 0)
        return;

    renderer->submit(
        Model(mesh.get(), &material), Mat3::identity(), layer, render_pass
    );
}

void
StaticBatch::clear()
{
    mesh_data.vertices.clear();
    mesh_data.indices.clear();
    if (mesh)
        mesh->update(mesh_data);
}

} // namespace lili
