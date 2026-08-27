#include "lili2d/render/scene/shapes/rect.hpp"

#include <cmath>

#include "lili2d/geometry/utils.hpp"
#include "lili2d/geometry/vec3.hpp"

namespace lili {

Rect::Rect(Renderer* renderer, RectShape shape, Vec4 color)
    : renderer(renderer) {
    mesh = renderer->getUnitQuad();
    material = std::make_unique<Material>(renderer->getTheWhitePixel());
    setShape(shape);
    setColor(color);
    ui_layout.offset = {shape.x, shape.y};
    rotation = 0.0f;
    layer = 0.0f;
}

void
Rect::setSize(Vec2 size) noexcept {
    if (shape.w != size.x || shape.h != size.y) {
        shape.w = size.x;
        shape.h = size.y;
        hollow_dirty = true;
    }
}

void
Rect::setShape(RectShape shape) noexcept {
    if (this->shape.w != shape.w || this->shape.h != shape.h) {
        hollow_dirty = true;
    }
    this->shape = shape;
    ui_layout.offset = {shape.x, shape.y};
}

Mat3
Rect::getTransformMatrix() const {
    if (render_layer == RenderLayer::UI && renderer) {
        Vec2 viewport_size = renderer->getLogicalResolution();
        Vec2 obj_size = {shape.w * scale.x, shape.h * scale.y};
        return ui_layout.getTransformationMatrix(
            viewport_size, obj_size, rotation, obj_size
        );
    }
    return Mat3::translate({shape.x, shape.y}) * Mat3::rotation(rotation) *
           Mat3::scale({shape.w * scale.x, shape.h * scale.y});
}

void
Rect::draw() {
    if (!is_visible) return;

    Mat3 mat_transform = getTransformMatrix();

    if (is_hollow) {
        if (hollow_dirty) {
            float w = (shape.w > 0.0001f) ? shape.w : 1.0f;
            float h = (shape.h > 0.0001f) ? shape.h : 1.0f;
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

            // Top quad
            add_quad(0.0f, 0.0f, 1.0f, ty);
            // Bottom quad
            add_quad(0.0f, 1.0f - ty, 1.0f, ty);
            // Left quad
            add_quad(0.0f, ty, tx, 1.0f - 2.0f * ty);
            // Right quad
            add_quad(1.0f - tx, ty, tx, 1.0f - 2.0f * ty);

            MeshData md;
            md.vertices = std::move(vertices);
            md.indices = std::move(indices);

            if (!hollow_mesh) {
                hollow_mesh =
                    std::make_unique<GPUMesh>(renderer->getDevice(), md);
            } else {
                hollow_mesh->update(md);
            }
            hollow_dirty = false;
        }

        renderer->submit(
            Model({hollow_mesh.get(), getMaterial()}), mat_transform, layer,
            render_layer
        );
    } else {
        renderer->submit(
            Model({mesh, getMaterial()}), mat_transform, layer, render_layer
        );
    }
}

}  // namespace lili
