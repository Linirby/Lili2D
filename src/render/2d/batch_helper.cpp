#include "lili2d/render/2d/batch_helper.hpp"
#include "lili2d/geometry/vec2.hpp"
#include "lili2d/geometry/vec3.hpp"
#include "lili2d/render/gpu/gpu_mesh.hpp"
#include <cmath>
#include <cstdint>

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
        Vec3(tp0.x, tp0.y, 0.0f), Vec2(slice.u_min, slice.v_min), 0, tint
    );
    mesh_data.vertices.emplace_back(
        Vec3(tp1.x, tp1.y, 0.0f), Vec2(slice.u_max, slice.v_min), 0, tint
    );
    mesh_data.vertices.emplace_back(
        Vec3(tp2.x, tp2.y, 0.0f), Vec2(slice.u_max, slice.v_max), 0, tint
    );
    mesh_data.vertices.emplace_back(
        Vec3(tp3.x, tp3.y, 0.0f), Vec2(slice.u_min, slice.v_max), 0, tint
    );

    mesh_data.indices.push_back(current_vertex_count + 0);
    mesh_data.indices.push_back(current_vertex_count + 1);
    mesh_data.indices.push_back(current_vertex_count + 2);
    mesh_data.indices.push_back(current_vertex_count + 2);
    mesh_data.indices.push_back(current_vertex_count + 3);
    mesh_data.indices.push_back(current_vertex_count + 0);
}

void
appendRect(
    MeshData& mesh_data,
    Vec2 pos,
    Vec2 size,
    Vec2 scale,
    float rotation,
    Vec4 color
)
{
    uint32_t base_idx = mesh_data.vertices.size();

    float cos_r = 1.0f;
    float sin_r = 0.0f;

    if (rotation != 0.0f) {
        float rot_rad = lili::degToRad(rotation);
        cos_r = std::cos(rot_rad);
        sin_r = std::sin(rot_rad);
    }

    float half_w = (size.x / 2.0f) * scale.x;
    float half_h = (size.y / 2.0f) * scale.y;

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
        Vec3(tp0.x, tp0.y, 0.0f), Vec2(0.0f, 0.0f), 0, color
    );
    mesh_data.vertices.emplace_back(
        Vec3(tp1.x, tp1.y, 0.0f), Vec2(1.0f, 0.0f), 0, color
    );
    mesh_data.vertices.emplace_back(
        Vec3(tp2.x, tp2.y, 0.0f), Vec2(1.0f, 1.0f), 0, color
    );
    mesh_data.vertices.emplace_back(
        Vec3(tp3.x, tp3.y, 0.0f), Vec2(0.0f, 1.0f), 0, color
    );

    mesh_data.indices.push_back(base_idx + 0);
    mesh_data.indices.push_back(base_idx + 1);
    mesh_data.indices.push_back(base_idx + 2);
    mesh_data.indices.push_back(base_idx + 2);
    mesh_data.indices.push_back(base_idx + 3);
    mesh_data.indices.push_back(base_idx + 0);
}

void
appendHollowRect(
    MeshData& mesh_data,
    Vec2 pos,
    Vec2 size,
    Vec2 scale,
    float rotation,
    Vec4 color,
    float thickness
)
{
    float half_w = (size.x / 2.0f) * scale.x;
    float half_h = (size.y / 2.0f) * scale.y;

    if (half_w <= 0.0f || half_h <= 0.0f || thickness <= 0.0f)
        return;

    float th_x = std::min(half_w, thickness);
    float th_y = std::min(half_h, thickness);
    if (th_x >= half_w || th_y >= half_h) {
        appendRect(mesh_data, pos, size, scale, rotation, color);
        return;
    }

    float cos_r = 1.0f;
    float sin_r = 0.0f;
    if (rotation != 0.0f) {
        float rot_rad = lili::degToRad(rotation);
        cos_r = std::cos(rot_rad);
        sin_r = std::sin(rot_rad);
    }

    auto transform_point = [&](Vec2 p) -> Vec2 {
        return Vec2(
            pos.x + p.x * cos_r - p.y * sin_r, pos.y + p.x * sin_r + p.y * cos_r
        );
    };

    auto add_quad = [&](Vec2 min_p, Vec2 max_p) {
        uint32_t base_idx = static_cast<uint32_t>(mesh_data.vertices.size());

        Vec2 tp0 = transform_point({ min_p.x, min_p.y });
        Vec2 tp1 = transform_point({ max_p.x, min_p.y });
        Vec2 tp2 = transform_point({ max_p.x, max_p.y });
        Vec2 tp3 = transform_point({ min_p.x, max_p.y });

        mesh_data.vertices.emplace_back(
            Vec3(tp0.x, tp0.y, 0.0f), Vec2(0.0f, 0.0f), 0, color
        );
        mesh_data.vertices.emplace_back(
            Vec3(tp1.x, tp1.y, 0.0f), Vec2(1.0f, 0.0f), 0, color
        );
        mesh_data.vertices.emplace_back(
            Vec3(tp2.x, tp2.y, 0.0f), Vec2(1.0f, 1.0f), 0, color
        );
        mesh_data.vertices.emplace_back(
            Vec3(tp3.x, tp3.y, 0.0f), Vec2(0.0f, 1.0f), 0, color
        );

        mesh_data.indices.push_back(base_idx + 0);
        mesh_data.indices.push_back(base_idx + 1);
        mesh_data.indices.push_back(base_idx + 2);
        mesh_data.indices.push_back(base_idx + 2);
        mesh_data.indices.push_back(base_idx + 3);
        mesh_data.indices.push_back(base_idx + 0);
    };

    add_quad({ -half_w, -half_h }, { half_w, -half_h + th_y });
    add_quad({ -half_w, half_h - th_y }, { half_w, half_h });
    add_quad({ -half_w, -half_h + th_y }, { -half_w + th_x, half_h - th_y });
    add_quad({ half_w - th_x, -half_h + th_y }, { half_w, half_h - th_y });
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
    const float PI = 3.14159265359f;

    int n = (segments >= 3) ? segments : 3;
    float angle_step = (2.0f * PI) / n;
    uint32_t base_idx = mesh_data.vertices.size();

    mesh_data.vertices.emplace_back(
        Vec3(center.x, center.y, 0.0f), Vec2(0.5f, 0.5f), 0.0f, color
    );

    for (int i = 0; i < n; i++) {
        float angle = i * angle_step;
        float cos_a = std::cos(angle);
        float sin_a = std::sin(angle);
        mesh_data.vertices.emplace_back(
            Vec3(center.x + radius * cos_a, center.y + radius * sin_a, 0.0f),
            Vec2(0.5f + 0.5f * cos_a, 0.5f + 0.5f * sin_a),
            0,
            color
        );
    }

    for (int i = 1; i <= n; i++) {
        uint32_t current_idx = i;
        uint32_t next_idx = (i == n) ? 1 : (i + 1);

        mesh_data.indices.push_back(base_idx);
        mesh_data.indices.push_back(base_idx + current_idx);
        mesh_data.indices.push_back(base_idx + next_idx);
    }
}

void
appendHollowCircle(
    MeshData& mesh_data,
    Vec2 center,
    float radius,
    float segments,
    Vec4 color,
    float thickness
)
{
    const float PI = 3.14159265359f;

    uint32_t base_idx = mesh_data.vertices.size();
    int n = (segments >= 3) ? segments : 3;
    float d = (radius + radius >= 1.0f) ? radius + radius : 1.0f;
    float inner_r = std::max(0.0f, radius - thickness);
    float angle_step = (2.0f * PI) / n;

    if (inner_r == 0.0f) {
        appendCircle(mesh_data, center, radius, segments, color);
        return;
    }

    for (int i = 0; i < n; i++) {
        float angle = i * angle_step;
        float cos_a = std::cos(angle);
        float sin_a = std::sin(angle);
        mesh_data.vertices.emplace_back(
            Vec3(center.x + inner_r * cos_a, center.y + inner_r * sin_a, 0.0f),
            Vec2(
                0.5f + inner_r / radius * cos_a, 0.5f + inner_r / radius * sin_a
            ),
            0,
            color
        );
        mesh_data.vertices.emplace_back(
            Vec3(center.x + radius * cos_a, center.y + radius * sin_a, 0.0f),
            Vec2(0.5f + cos_a, 0.5f + sin_a),
            0,
            color
        );

        uint32_t current_inner = i * 2;
        uint32_t current_outer = i * 2 + 1;
        uint32_t next_inner = ((i + 1) % n) * 2;
        uint32_t next_outer = ((i + 1) % n) * 2 + 1;

        mesh_data.indices.push_back(base_idx + current_inner);
        mesh_data.indices.push_back(base_idx + current_outer);
        mesh_data.indices.push_back(base_idx + next_inner);

        mesh_data.indices.push_back(base_idx + next_inner);
        mesh_data.indices.push_back(base_idx + current_outer);
        mesh_data.indices.push_back(base_idx + next_outer);
    }
}

void
appendLine(
    MeshData& mesh_data,
    Vec2 start_pos,
    Vec2 dxy,
    float thickness,
    Vec4 color
)
{
    uint32_t base_idx = mesh_data.vertices.size();

    float len = std::sqrt(dxy.x * dxy.x + dxy.y * dxy.y);
    if (len < 1.0f)
        return;
    float half_t = (thickness >= 1.0f) ? thickness * 0.5f : 0.5f;
    float ox = -half_t * (dxy.y / len);
    float oy = half_t * (dxy.x / len);

    mesh_data.vertices.emplace_back(
        Vec3(start_pos.x + ox, start_pos.y + oy, 0.0f),
        Vec2(0.0f, 0.0f),
        0,
        color
    );
    mesh_data.vertices.emplace_back(
        Vec3(start_pos.x + dxy.x + ox, start_pos.y + dxy.y + oy, 0.0f),
        Vec2(1.0f, 0.0f),
        0,
        color
    );
    mesh_data.vertices.emplace_back(
        Vec3(start_pos.x + dxy.x - ox, start_pos.y + dxy.y - oy, 0.0f),
        Vec2(1.0f, 1.0f),
        0,
        color
    );
    mesh_data.vertices.emplace_back(
        Vec3(start_pos.x - ox, start_pos.y - oy, 0.0f),
        Vec2(0.0f, 1.0f),
        0,
        color
    );

    mesh_data.indices.push_back(base_idx + 0);
    mesh_data.indices.push_back(base_idx + 1);
    mesh_data.indices.push_back(base_idx + 2);
    mesh_data.indices.push_back(base_idx + 2);
    mesh_data.indices.push_back(base_idx + 3);
    mesh_data.indices.push_back(base_idx + 0);
}

void
appendText(
    MeshData& mesh_data,
    Vec2 pos,
    uint32_t length,
    float spacing,
    Vec4 color
)
{
}

} // namespace lili::BatchHelper
