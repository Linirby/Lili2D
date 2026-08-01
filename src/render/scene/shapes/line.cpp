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
Line::setPosition(Vec2 pos) {
    Vec2 delta = pos - shape.start;
    shape.start = pos;
    shape.end = shape.end + delta;
    ui_layout.offset = pos;
}

void
Line::setRotation(float degree) {
    Vec2 diff = shape.end - shape.start;
    float length = diff.length();
    float rad = lili::degToRad(degree);
    shape.end =
        shape.start + Vec2(std::cos(rad) * length, std::sin(rad) * length);
}

void
Line::setScale(Vec2 scale) {
    this->scale = scale;
}

void
Line::setSize(Vec2 size) {
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

void
Line::setStart(Vec2 pos) {
    shape.start = pos;
    ui_layout.offset = pos;
}

void
Line::setEnd(Vec2 pos) {
    shape.end = pos;
}

void
Line::setThickness(float value) {
    shape.thickness = value;
}

void
Line::setShape(LineShape shape) {
    this->shape = shape;
    ui_layout.offset = shape.start;
}

void
Line::setColor(Vec4 color) {
    if (material) {
        material->properties.color_tint = color;
    }
}

void
Line::setMaterial(Material* material) {
    external_material = material;
}

void
Line::setLayer(float value) {
    layer = value;
}

void
Line::setRender(RenderLayer render_layer) {
    this->render_layer = render_layer;
}

Vec2
Line::getPosition() const {
    return shape.start;
}

float
Line::getRotation() const {
    Vec2 diff = shape.end - shape.start;
    float angle = std::atan2(diff.y, diff.x);
    return lili::radToDeg(angle);
}

Vec2
Line::getScale() const {
    return scale;
}

Vec2
Line::getSize() const {
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
        Vec2 viewport_size = {
            static_cast<float>(renderer->getSwapchainWidth()),
            static_cast<float>(renderer->getSwapchainHeight())
        };
        Vec2 obj_size = {length, thick};
        return ui_layout.getTransformationMatrix(
            viewport_size, obj_size, angle, scale
        );
    }

    return Mat3::translate(shape.start) * Mat3::rotation(angle) *
           Mat3::translate({0.0f, -thick * 0.5f}) *
           Mat3::scale({length, thick});
}

float
Line::getLayer() const {
    return layer;
}

RenderLayer
Line::getRender() const {
    return render_layer;
}

Vec2
Line::getStart() const {
    return shape.start;
}

Vec2
Line::getEnd() const {
    return shape.end;
}

float
Line::getThickness() const {
    return shape.thickness;
}

LineShape
Line::getShape() const {
    return shape;
}

Vec4
Line::getColor() const {
    Material* mat = getMaterial();
    return mat ? mat->properties.color_tint : Vec4(1, 1, 1, 1);
}

Material*
Line::getMaterial() const {
    return external_material ? external_material : material.get();
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
