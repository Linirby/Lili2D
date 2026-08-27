#include "lili2d/render/scene/shapes/line.hpp"

#include <cmath>

#include "lili2d/geometry/utils.hpp"

namespace lili {

Line::Line(Renderer* renderer, LineShape shape, Vec4 color)
    : renderer(renderer) {
    mesh = renderer->getUnitQuad();
    material = std::make_unique<Material>(renderer->getTheWhitePixel());
    setShape(shape);
    setColor(color);
    ui_layout.offset = shape.start;
    layer = 0.0f;
}

void
Line::setPosition(Vec2 pos) noexcept {
    Vec2 delta = pos - shape.start;
    shape.start = pos;
    shape.end = shape.end + delta;
    ui_layout.offset = pos;
}

void
Line::setRotation(float degree) noexcept {
    Vec2 diff = shape.end - shape.start;
    float length = diff.length();
    float rad = lili::degToRad(degree);
    shape.end =
        shape.start + Vec2(std::cos(rad) * length, std::sin(rad) * length);
}

void
Line::setSize(Vec2 size) noexcept {
    Vec2 diff = shape.end - shape.start;
    float current_len = diff.length();
    if (current_len > 0.0001f) {
        Vec2 dir = diff.normalized();
        shape.end = shape.start + dir * size.x;
    } else {
        shape.end = shape.start + Vec2(size.x, 0.0f);
    }
    shape.thickness = size.y;
}

float
Line::getRotation() const noexcept {
    Vec2 diff = shape.end - shape.start;
    float angle = std::atan2(diff.y, diff.x);
    return lili::radToDeg(angle);
}

Vec2
Line::getSize() const noexcept {
    Vec2 diff = shape.end - shape.start;
    return {diff.length() * scale.x, shape.thickness * scale.y};
}

Mat3
Line::getTransformMatrix() const {
    Vec2 diff = shape.end - shape.start;
    float length = diff.length() * scale.x;
    float thick = shape.thickness * scale.y;
    float angle = std::atan2(diff.y, diff.x);

    if (render_layer == RenderLayer::UI && renderer) {
        Vec2 viewport_size = renderer->getLogicalResolution();
        Vec2 obj_size = {length, thick};
        return ui_layout.getTransformationMatrix(
            viewport_size, obj_size, angle, scale
        );
    }

    return Mat3::translate(shape.start) * Mat3::rotation(angle) *
           Mat3::translate({0.0f, -thick * 0.5f}) *
           Mat3::scale({length, thick});
}

void
Line::draw() {
    if (!is_visible) return;
    Mat3 mat_transform = getTransformMatrix();
    renderer->submit(
        Model({mesh, getMaterial()}), mat_transform, layer, render_layer
    );
}

}  // namespace lili
