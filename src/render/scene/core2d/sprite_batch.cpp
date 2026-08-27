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

SpriteBatch::~SpriteBatch() {}

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

    Vertex v0;
    v0.x = tp0.x;
    v0.y = tp0.y;
    v0.z = 0.0f;
    v0.u = slice.u_min;
    v0.v = slice.v_min;
    v0.material_id = 0.0f;
    v0.r = color.x;
    v0.g = color.y;
    v0.b = color.z;
    v0.a = color.w;

    Vertex v1;
    v1.x = tp1.x;
    v1.y = tp1.y;
    v1.z = 0.0f;
    v1.u = slice.u_max;
    v1.v = slice.v_min;
    v1.material_id = 0.0f;
    v1.r = color.x;
    v1.g = color.y;
    v1.b = color.z;
    v1.a = color.w;

    Vertex v2;
    v2.x = tp2.x;
    v2.y = tp2.y;
    v2.z = 0.0f;
    v2.u = slice.u_max;
    v2.v = slice.v_max;
    v2.material_id = 0.0f;
    v2.r = color.x;
    v2.g = color.y;
    v2.b = color.z;
    v2.a = color.w;

    Vertex v3;
    v3.x = tp3.x;
    v3.y = tp3.y;
    v3.z = 0.0f;
    v3.u = slice.u_min;
    v3.v = slice.v_max;
    v3.material_id = 0.0f;
    v3.r = color.x;
    v3.g = color.y;
    v3.b = color.z;
    v3.a = color.w;

    mesh_data.vertices.push_back(v0);
    mesh_data.vertices.push_back(v1);
    mesh_data.vertices.push_back(v2);
    mesh_data.vertices.push_back(v3);

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

void
SpriteBatch::setColorTint(Vec4 color) {
    if (material) material->properties.color_tint = color;
}

void
SpriteBatch::setColor(Vec4 color) {
    setColorTint(color);
}

void
SpriteBatch::setMaterial(Material* material) {
    external_material = material;
}

void
SpriteBatch::setPosition(Vec2 position) {
    this->position = position;
    ui_layout.offset = position;
}

void
SpriteBatch::setRotation(float degree) {
    rotation = lili::degToRad(degree);
}

void
SpriteBatch::setScale(Vec2 scale) {
    this->scale = scale;
}

void
SpriteBatch::setSize(Vec2 size) {
    custom_size = size;
}

void
SpriteBatch::setLayer(float layer) {
    this->layer = layer;
}

Vec2
SpriteBatch::getPosition() const {
    return position;
}

float
SpriteBatch::getRotation() const {
    return lili::radToDeg(rotation);
}

Vec2
SpriteBatch::getScale() const {
    return scale;
}

Vec2
SpriteBatch::getSize() const {
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

float
SpriteBatch::getLayer() const {
    return layer;
}

Vec4
SpriteBatch::getColor() const {
    Material* mat = getMaterial();
    return mat ? mat->properties.color_tint : Vec4(1, 1, 1, 1);
}

Material*
SpriteBatch::getMaterial() const {
    return external_material ? external_material : material.get();
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
