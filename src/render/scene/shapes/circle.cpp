#include "lili2d/render/scene/shapes/circle.hpp"

#include <cmath>

#include "lili2d/geometry/utils.hpp"
#include "lili2d/geometry/vec3.hpp"
#include "lili2d/render/renderer.hpp"

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

bool
Circle::containsPoint(Vec2 point, const Renderer* renderer) const {
    if (!is_visible) return false;
    (void)renderer;
    Mat3 inv_mat = getTransformMatrix().inverse();
    Vec2 local_pt = inv_mat.transformPoint(point);
    return local_pt.length() <= 0.5f;
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
                vertices.emplace_back(
                    Vec3(inner_r * cos_a, inner_r * sin_a, 0.0f),
                    Vec2(inner_r * cos_a + 0.5f, inner_r * sin_a + 0.5f)
                );
                vertices.emplace_back(
                    Vec3(outer_r * cos_a, outer_r * sin_a, 0.0f),
                    Vec2(outer_r * cos_a + 0.5f, outer_r * sin_a + 0.5f)
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

            MeshData md;
            md.vertices = std::move(vertices);
            md.indices = std::move(indices);

            if (!hollow_mesh)
                hollow_mesh =
                    std::make_unique<GPUMesh>(renderer->getDevice(), md);
            else
                hollow_mesh->update(md);
            hollow_dirty = false;
        }

        renderer->submit(
            Model({hollow_mesh.get(), getMaterial()}), mat_transform, layer,
            render_layer
        );
    } else
        renderer->submit(
            Model({mesh, getMaterial()}), mat_transform, layer, render_layer
        );
}

}  // namespace lili
