#include "lili2d/render/scene/shapes/circle.hpp"

#include <cmath>

#include "lili2d/geometry/utils.hpp"
#include "lili2d/geometry/vec3.hpp"

namespace lili {

Circle::Circle(Renderer* renderer, CircleShape shape, Vec4 color)

    : renderer(renderer) {
    mesh = renderer->getUnitCircle(shape.segments);
    material = std::make_unique<Material>(renderer->getTheWhitePixel());
    setShape(shape);
    setColor(color);
    ui_layout.offset = shape.center;
    layer = 0.0f;
}

void
Circle::setPosition(Vec2 pos) {
    shape.center = pos;
    ui_layout.offset = pos;
}

void
Circle::setRotation(float degree) {
    rotation = lili::degToRad(degree);
}

void
Circle::setScale(Vec2 scale) {
    this->scale = scale;
}

void
Circle::setSize(Vec2 size) {
    float max_dim = std::max(size.x, size.y);
    setRadius(max_dim * 0.5f);
}

void
Circle::setCenter(Vec2 pos) {
    shape.center = pos;
    ui_layout.offset = pos;
}

void
Circle::setRadius(float r) {
    if (shape.radius != r) {
        shape.radius = r;
        hollow_dirty = true;
    }
}

void
Circle::setSegments(int n) {
    if (shape.segments != n) {
        shape.segments = n;
        mesh = renderer->getUnitCircle(n);
        hollow_dirty = true;
    }
}

void
Circle::setShape(CircleShape shape) {
    this->shape = shape;
    ui_layout.offset = shape.center;
    mesh = renderer->getUnitCircle(shape.segments);
    hollow_dirty = true;
}

void
Circle::setColor(Vec4 color) {
    if (material) material->properties.color_tint = color;
}

void
Circle::setMaterial(Material* material) {
    external_material = material;
}

void
Circle::setHollow(bool hollow) {
    is_hollow = hollow;
}

void
Circle::setHollowThickness(float thickness) {
    if (hollow_thickness != thickness) {
        hollow_thickness = thickness;
        hollow_dirty = true;
    }
}

void
Circle::setLayer(float value) {
    layer = value;
}

void
Circle::setRender(RenderLayer render_layer) {
    this->render_layer = render_layer;
}

Vec2
Circle::getPosition() const {
    return shape.center;
}

float
Circle::getRotation() const {
    return lili::radToDeg(rotation);
}

Vec2
Circle::getScale() const {
    return scale;
}

Vec2
Circle::getSize() const {
    float d = getDiameter();
    return {d, d};
}

Mat3
Circle::getTransformMatrix() const {
    if (render_layer == RenderLayer::UI && renderer) {
        Vec2 viewport_size = renderer->getLogicalResolution();
        Vec2 obj_size = {getDiameter() * scale.x, getDiameter() * scale.y};
        Vec2 top_left = ui_layout.getScreenPosition(viewport_size, obj_size);
        Vec2 center_pos = top_left + obj_size * 0.5f;
        return Mat3::translate(center_pos) * Mat3::rotation(rotation) *
               Mat3::scale(obj_size);
    }
    return Mat3::translate(shape.center) * Mat3::rotation(rotation) *
           Mat3::scale({getDiameter() * scale.x, getDiameter() * scale.y});
}

float
Circle::getLayer() const {
    return layer;
}

RenderLayer
Circle::getRender() const {
    return render_layer;
}

bool
Circle::containsPoint(Vec2 point, const Renderer* renderer) const {
    if (!is_visible) return false;
    (void)renderer;
    Mat3 inv_mat = getTransformMatrix().inverse();
    Vec2 local_pt = inv_mat.transformPoint(point);
    return local_pt.length() <= 0.5f;
}

Vec2
Circle::getCenter() const {
    return shape.center;
}

Vec2
Circle::getTopLeft() const {
    return shape.center - Vec2(shape.radius, shape.radius);
}

float
Circle::getRadius() const {
    return shape.radius;
}

float
Circle::getDiameter() const {
    return shape.radius * 2.0f;
}

CircleShape
Circle::getShape() const {
    return shape;
}

Vec4
Circle::getColor() const {
    Material* mat = getMaterial();
    return mat ? mat->properties.color_tint : Vec4(1, 1, 1, 1);
}

Material*
Circle::getMaterial() const {
    return external_material ? external_material : material.get();
}

bool
Circle::isHollow() const {
    return is_hollow;
}

float
Circle::getHollowThickness() const {
    return hollow_thickness;
}

void
Circle::draw() {
    if (!is_visible) return;

    Mat3 mat_transform = getTransformMatrix();

    if (is_hollow) {
        if (hollow_dirty) {
            int n = (shape.segments >= 3) ? shape.segments : 3;
            float d = getDiameter();
            if (d < 0.0001f) d = 1.0f;
            float outer_r = 0.5f;
            float inner_r = std::max(0.0f, 0.5f - (hollow_thickness / d));

            const float PI = 3.14159265359f;
            float angle_step = (2.0f * PI) / n;

            std::vector<Vertex> vertices;
            std::vector<uint32_t> indices;

            for (int i = 0; i < n; i++) {
                float angle = i * angle_step;
                float cos_a = std::cos(angle);
                float sin_a = std::sin(angle);
                vertices.push_back(
                    Vertex{
                        inner_r * cos_a, inner_r * sin_a, 0.0f,
                        inner_r * cos_a + 0.5f, inner_r * sin_a + 0.5f
                    }
                );
                vertices.push_back(
                    Vertex{
                        outer_r * cos_a, outer_r * sin_a, 0.0f,
                        outer_r * cos_a + 0.5f, outer_r * sin_a + 0.5f
                    }
                );

                uint32_t current_inner = i * 2;
                uint32_t current_outer = i * 2 + 1;
                uint32_t next_inner = ((i + 1) % n) * 2;
                uint32_t next_outer = ((i + 1) % n) * 2 + 1;

                indices.push_back(current_inner);
                indices.push_back(current_outer);
                indices.push_back(next_inner);

                indices.push_back(next_inner);
                indices.push_back(current_outer);
                indices.push_back(next_outer);
            }

            MeshData mesh_data;
            mesh_data.vertices = vertices;
            mesh_data.indices = indices;
            if (!hollow_mesh)
                hollow_mesh =
                    std::make_unique<GPUMesh>(renderer->getDevice(), mesh_data);
            else
                hollow_mesh->update(mesh_data);
            hollow_dirty = false;
        }

        renderer->submit(
            Model(hollow_mesh.get(), getMaterial()), mat_transform, layer,
            render_layer
        );
    } else
        renderer->submit(
            Model(mesh, getMaterial()), mat_transform, layer, render_layer
        );
}

}  // namespace lili
