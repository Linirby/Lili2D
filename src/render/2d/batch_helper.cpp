#include "lili2d/render/2d/batch_helper.hpp"

namespace lili::BatchHelper {

void
appendSprite(
    MeshData& mesh_data,
    const SliceUV& slice,
    Vec2 pos,
    Vec2 scale,
    float rotation,
    Vec4 tint
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
        Vec3(tp0.x, tp0.y, 0.0f), Vec2(slice.u_min, slice.v_min), 0.0f, tint
    );
    mesh_data.vertices.emplace_back(
        Vec3(tp1.x, tp1.y, 0.0f), Vec2(slice.u_max, slice.v_min), 0.0f, tint
    );
    mesh_data.vertices.emplace_back(
        Vec3(tp2.x, tp2.y, 0.0f), Vec2(slice.u_max, slice.v_max), 0.0f, tint
    );
    mesh_data.vertices.emplace_back(
        Vec3(tp3.x, tp3.y, 0.0f), Vec2(slice.u_min, slice.v_max), 0.0f, tint
    );

    mesh_data.indices.push_back(current_vertex_count + 0);
    mesh_data.indices.push_back(current_vertex_count + 1);
    mesh_data.indices.push_back(current_vertex_count + 2);
    mesh_data.indices.push_back(current_vertex_count + 2);
    mesh_data.indices.push_back(current_vertex_count + 3);
    mesh_data.indices.push_back(current_vertex_count + 0);
}

void
appendRect(MeshData& mesh_data, Vec2 pos, Vec2 size, float rotation, Vec4 color)
{
}

void
appendHollowRect(
    MeshData& mesh_data,
    Vec2 pos,
    Vec2 size,
    float thickness,
    float rotation,
    Vec4 color
)
{
}

void
appendCircle(
    MeshData& mesh_data,
    Vec2 center,
    float radius,
    float segments,
    Vec4 color
)
{
}

void
appendHollowCircle(
    MeshData& mesh_data,
    Vec2 center,
    float radius,
    float thickness,
    float segments,
    Vec4 color
)
{
}

void
appendLine(MeshData& mesh_data, Vec2 dxy, float thickness, Vec4 color)
{
}

} // namespace lili::BatchHelper
