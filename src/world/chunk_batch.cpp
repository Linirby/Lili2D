#include "lili2d/world/chunk_batch.hpp"

#include <memory>

#include "lili2d/geometry/mat3x3.hpp"
#include "lili2d/render/2d/batch_helper.hpp"
#include "lili2d/render/common/material.hpp"
#include "lili2d/render/common/model.hpp"
#include "lili2d/render/gpu/gpu_mesh.hpp"
#include "lili2d/render/gpu/pass_types.hpp"
#include "lili2d/render/renderer.hpp"

namespace lili {

ChunkBatch::ChunkBatch(Renderer* renderer, Texture* texture)
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
ChunkBatch::add(
    const SliceUV& slice,
    Vec2 pos,
    Vec2 scale,
    float rotation,
    Vec4 color
)
{
    BatchHelper::appendSprite(mesh_data, slice, pos, scale, rotation, color);
    if (active_texture != slice.texture)
        active_texture = slice.texture;
}

void
ChunkBatch::end()
{
    if (!mesh_data.vertices.empty()) {
        if (batch_pool.empty()) {
            batch_pool.push_back(
                BatchItem{ .mesh = std::make_unique<GPUMesh>(
                               renderer->getDevice(), mesh_data
                           ),
                           .material = Material(active_texture) }
            );
        } else {
            batch_pool[0].mesh->update(mesh_data);
            batch_pool[0].material.albedoMap = active_texture;
        }
        mesh_data.vertices.clear();
        mesh_data.indices.clear();
    }
}

void
ChunkBatch::draw()
{
    if (!mesh_data.vertices.empty()) {
        if (batch_pool.empty())
            batch_pool.push_back(
                BatchItem{ .mesh = std::make_unique<GPUMesh>(
                               renderer->getDevice(), mesh_data
                           ),
                           .material = Material(active_texture) }
            );
        else {
            batch_pool[0].mesh->update(mesh_data);
            batch_pool[0].material.albedoMap = active_texture;
        }
        mesh_data.vertices.clear();
        mesh_data.indices.clear();
    }

    if (batch_pool.empty() || !batch_pool[0].mesh ||
        batch_pool[0].mesh->getIndexCount() == 0)
        return;

    renderer->submit(
        Model(batch_pool[0].mesh.get(), &batch_pool[0].material),
        Mat3::identity(),
        layer,
        RenderLayer::WORLD2D
    );
}

void
ChunkBatch::setMeshData(MeshData&& data)
{
    mesh_data = std::move(data);
    if (batch_pool.empty())
        batch_pool.push_back(
            BatchItem{
                .mesh =
                    std::make_unique<GPUMesh>(renderer->getDevice(), mesh_data),
                .material = Material(active_texture),
            }
        );
    else {
        batch_pool[0].mesh->update(mesh_data);
        batch_pool[0].material.albedoMap = active_texture;
    }
    mesh_data.vertices.clear();
    mesh_data.indices.clear();
}

} // namespace lili
