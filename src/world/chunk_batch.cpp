#include "lili2d/world/chunk_batch.hpp"

#include <cmath>
#include <memory>

#include "lili2d/geometry/mat3x3.hpp"
#include "lili2d/geometry/utils.hpp"
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
    appendToMesh(mesh_data, slice, pos, scale, rotation, color);
    if (active_texture != slice.texture)
        active_texture = slice.texture;
}

void
ChunkBatch::appendToMesh(
    MeshData& mesh_data,
    const SliceUV& slice,
    Vec2 pos,
    Vec2 scale,
    float rotation,
    Vec4 color
)
{
    uint32_t current_vertex_count =
        static_cast<uint32_t>(mesh_data.vertices.size());

    float cos_r = 1.0f;
    float sin_r = 0.0f;

    if (rotation != 0.0f) {
        float rot_rad = lili::degToRad(rotation);
        cos_r = std::cos(rot_rad);
        sin_r = std::sin(rot_rad);
    }

    float half_w = (slice.width / 2.0f) * scale.x;
    float half_h = (slice.height / 2.0f) * scale.y;

    Vec2 p0{ -half_w, -half_h };
    Vec2 p1{ half_w, -half_h };
    Vec2 p2{ half_w, half_h };
    Vec2 p3{ -half_w, half_h };

    auto transform_point = [&](Vec2 p) -> Vec2 {
        return Vec2(
            pos.x + p.x * cos_r - p.y * sin_r, pos.y + p.x * sin_r + p.y * cos_r
        );
    };

    Vec2 tp0 = transform_point(p0);
    Vec2 tp1 = transform_point(p1);
    Vec2 tp2 = transform_point(p2);
    Vec2 tp3 = transform_point(p3);

    mesh_data.vertices.emplace_back(
        Vec3(tp0.x, tp0.y, 0.0f), Vec2(slice.u_min, slice.v_min), 0.0f, color
    );
    mesh_data.vertices.emplace_back(
        Vec3(tp1.x, tp1.y, 0.0f), Vec2(slice.u_max, slice.v_min), 0.0f, color
    );
    mesh_data.vertices.emplace_back(
        Vec3(tp2.x, tp2.y, 0.0f), Vec2(slice.u_max, slice.v_max), 0.0f, color
    );
    mesh_data.vertices.emplace_back(
        Vec3(tp3.x, tp3.y, 0.0f), Vec2(slice.u_min, slice.v_max), 0.0f, color
    );

    mesh_data.indices.push_back(current_vertex_count + 0);
    mesh_data.indices.push_back(current_vertex_count + 1);
    mesh_data.indices.push_back(current_vertex_count + 2);
    mesh_data.indices.push_back(current_vertex_count + 2);
    mesh_data.indices.push_back(current_vertex_count + 3);
    mesh_data.indices.push_back(current_vertex_count + 0);
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
    if (batch_pool.empty()) {
        batch_pool.push_back(
            BatchItem{
                .mesh =
                    std::make_unique<GPUMesh>(renderer->getDevice(), mesh_data),
                .material = Material(active_texture) }
        );
    } else {
        batch_pool[0].mesh->update(mesh_data);
        batch_pool[0].material.albedoMap = active_texture;
    }
    mesh_data.vertices.clear();
    mesh_data.indices.clear();
}

} // namespace lili
