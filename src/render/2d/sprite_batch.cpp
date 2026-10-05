#include "lili2d/render/2d/sprite_batch.hpp"

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

SpriteBatch::SpriteBatch(Renderer* renderer, Texture* texture)
  : renderer(renderer)
{
    batch_pool.push_back(
        { .mesh = std::make_unique<GPUMesh>(renderer->getDevice(), mesh_data),
          .material = Material(texture) }
    );
    active_texture = texture;
}

void
SpriteBatch::draw(
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
SpriteBatch::appendToMesh(
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
SpriteBatch::flush(float layer, RenderLayer render_pass)
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
    current_item.mesh->update(mesh_data);
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

void
SpriteBatch::draw()
{
    flush(layer, RenderLayer::WORLD2D);
}

void
SpriteBatch::setMeshData(MeshData&& data)
{
    mesh_data = std::move(data);
    if (batch_pool.empty())
        batch_pool.push_back(
            BatchItem{
                .mesh =
                    std::make_unique<GPUMesh>(renderer->getDevice(), mesh_data),
                .material = Material(active_texture) }
        );
    else
        batch_pool[0].mesh->update(mesh_data);
}

} // namespace lili
