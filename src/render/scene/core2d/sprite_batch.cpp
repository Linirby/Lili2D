#include "lili2d/render/scene/core2d/sprite_batch.hpp"

#include <cmath>

#include "lili2d/geometry/mat3x3.hpp"
#include "lili2d/geometry/utils.hpp"

namespace lili {

SpriteBatch::SpriteBatch(Renderer* renderer, Texture* texture)
    : renderer(renderer) {
    material = std::make_unique<Material>(texture);
    material->properties.color_tint = Vec4(1.0f, 1.0f, 1.0f, 1.0f);

    mesh = std::make_unique<GPUMesh>(renderer->getDevice(), mesh_data);

    position = Vec2(0.0f, 0.0f);
    scale = Vec2(1.0f, 1.0f);
    layer = 0.0f;
}

void
SpriteBatch::begin() {
    mesh_data.vertices.clear();
    mesh_data.indices.clear();
}

void
SpriteBatch::clear() {
    mesh_data.vertices.clear();
    mesh_data.indices.clear();
    mesh->update(mesh_data);
}

void
SpriteBatch::appendSpriteToMesh(
    MeshData& mesh_data, const SliceUV& slice, Vec2 pos, Vec2 scale,
    float rotation, Vec4 color
) {
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

    Vec2 p0{-half_w, -half_h};
    Vec2 p1{half_w, -half_h};
    Vec2 p2{half_w, half_h};
    Vec2 p3{-half_w, half_h};

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
SpriteBatch::setMeshData(MeshData&& data) {
    mesh_data = std::move(data);
    mesh->update(mesh_data);
}

void
SpriteBatch::draw(
    const SliceUV& slice, Vec2 pos, Vec2 scale, float rotation, Vec4 color
) {
    appendSpriteToMesh(mesh_data, slice, pos, scale, rotation, color);
}

void
SpriteBatch::end() {
    mesh->update(mesh_data);
}

Vec2
SpriteBatch::getSize() const noexcept {
    if (custom_size.x > 0.0f || custom_size.y > 0.0f)
        return Vec2(custom_size.x * scale.x, custom_size.y * scale.y);
    if (mesh_data.vertices.empty()) return Vec2(0.0f, 0.0f);

    float min_x = mesh_data.vertices[0].x;
    float max_x = mesh_data.vertices[0].x;
    float min_y = mesh_data.vertices[0].y;
    float max_y = mesh_data.vertices[0].y;

    for (const Vertex& v : mesh_data.vertices) {
        if (v.x < min_x) min_x = v.x;
        if (v.x > max_x) max_x = v.x;
        if (v.y < min_y) min_y = v.y;
        if (v.y > max_y) max_y = v.y;
    }

    return Vec2((max_x - min_x) * scale.x, (max_y - min_y) * scale.y);
}

Mat3
SpriteBatch::getTransformMatrix() const {
    if (render_layer == RenderLayer::UI && renderer) {
        Vec2 viewport_size = renderer->getLogicalResolution();
        Vec2 obj_size = getSize();
        return ui_layout.getTransformationMatrix(
            viewport_size, obj_size, rotation, scale
        );
    }
    return Mat3::translate(position) * Mat3::rotation(rotation) *
           Mat3::scale(scale);
}

void
SpriteBatch::draw() {
    if (mesh_data.indices.empty() || !is_visible) return;

    Mat3 mat_transform = getTransformMatrix();

    renderer->submit(
        Model(mesh.get(), getMaterial()), mat_transform, layer, render_layer
    );
}

}  // namespace lili
