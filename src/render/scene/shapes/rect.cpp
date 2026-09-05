#include "lili2d/render/scene/shapes/rect.hpp"

#include <cmath>

#include "lili2d/geometry/utils.hpp"
#include "lili2d/geometry/vec3.hpp"
#include "lili2d/render/renderer.hpp"

namespace lili {

Rect::Rect(Renderer* renderer, RectShape shape, Vec4 color)
    : renderer(renderer) {
    mesh = renderer->getUnitQuad();
    material = std::make_unique<Material>(renderer->getTheWhitePixel());
    setShape(shape);
    setColor(color);
    ui_layout.offset = shape.pos;
    rotation = 0.0f;
    layer = 0.0f;
}

void
Rect::setSize(Vec2 size) noexcept {
    if (shape.size != size) {
        shape.size = size;
        hollow_dirty = true;
    }
}

void
Rect::setShape(RectShape shape) noexcept {
    if (this->shape.size != shape.size)
        hollow_dirty = true;
    this->shape = shape;
    ui_layout.offset = shape.pos;
}

Mat3
Rect::getTransformMatrix() const {
    Vec2 obj_size = {shape.size.x * scale.x, shape.size.y * scale.y};
    if (render_layer == RenderLayer::UI && renderer) {
        Vec2 viewport_size = renderer->getLogicalResolution();
        return ui_layout.getTransformationMatrix(
            viewport_size, obj_size, rotation, obj_size
        );
    }
    return Mat3::translate(shape.pos) * Mat3::rotation(rotation) *
           Mat3::scale(obj_size);
}

void
Rect::draw() {
    if (!is_visible) return;

    Mat3 mat_transform = getTransformMatrix();

    if (is_hollow) {
        if (hollow_dirty) {
            float w = (shape.size.x > 0.0001f) ? shape.size.x : 1.0f;
            float h = (shape.size.y > 0.0001f) ? shape.size.y : 1.0f;
            float tx = std::min(0.5f, hollow_thickness / w);
            float ty = std::min(0.5f, hollow_thickness / h);

            std::vector<Vertex> vertices;
            std::vector<uint32_t> indices;

            auto add_quad = [&](float qx, float qy, float qw, float qh) {
                uint32_t start_idx = vertices.size();
                vertices.emplace_back(Vec3(qx, qy, 0.0f), Vec2(qx, qy));
                vertices.emplace_back(
                    Vec3(qx + qw, qy, 0.0f), Vec2(qx + qw, qy)
                );
                vertices.emplace_back(
                    Vec3(qx, qy + qh, 0.0f), Vec2(qx, qy + qh)
                );
                vertices.emplace_back(
                    Vec3(qx + qw, qy + qh, 0.0f), Vec2(qx + qw, qy + qh)
                );

                indices.push_back(start_idx + 0);
                indices.push_back(start_idx + 1);
                indices.push_back(start_idx + 2);
                indices.push_back(start_idx + 2);
                indices.push_back(start_idx + 1);
                indices.push_back(start_idx + 3);
            };

            // Top quad (hihi ^^)
            add_quad(0.0f, 0.0f, 1.0f, ty);
            // Bottom quad (0v0)
            add_quad(0.0f, 1.0f - ty, 1.0f, ty);
            // Left quad (<3)
            add_quad(0.0f, ty, tx, 1.0f - 2.0f * ty);
            // Right quad (0> pinguin)
            add_quad(1.0f - tx, ty, tx, 1.0f - 2.0f * ty);

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
