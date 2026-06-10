#include "render/scene/shapes/rect.hpp"

#include "geometry/utils.hpp"

namespace lili {

RectShape::RectShape(float x, float y, float w, float h)
	: x(x), y(y), w(w), h(h) {}

Rect::Rect(Renderer *renderer, RectShape shape, Vec4 color)
	: renderer(renderer) {
	mesh = renderer->getUnitQuad();
	material = std::make_unique<Material>(renderer->getTheWhitePixel());
	setShape(shape);
	setColor(color);
	rotation = 0.0f;
	layer = 0.0f;
}

void Rect::setPosition(Vec2 pos) {
	shape.x = pos.x;
	shape.y = pos.y;
}

void Rect::setRotation(float degree) {
	rotation = lili::degToRad(degree);
}

void Rect::setSize(Vec2 size) {
	shape.w = size.x;
	shape.h = size.y;
}

void Rect::setShape(RectShape shape) {
	this->shape = shape;
}

void Rect::setColor(Vec4 color) {
	material->properties.color_tint = color;
}

void Rect::setLayer(float value) {
	layer = value;
}

void Rect::setRender(RenderLayer render_layer) {
	this->render_layer = render_layer;
}

Vec2 Rect::getPosition() const {
	return { shape.x, shape.y };
}

Vec2 Rect::getSize() const {
	return { shape.w, shape.h };
}

RectShape Rect::getShape() const {
	return shape;
}

Vec4 Rect::getColor() const {
	return material->properties.color_tint;
}

Material* Rect::getMaterial() const {
	return material.get();
}

void Rect::draw() {
	Mat3 mat_transform = (
		Mat3::translate({ shape.x, shape.y }) *
		Mat3::rotation(rotation) *
		Mat3::scale({ shape.w, shape.h })
	);
	renderer->submit(
		(Model){ mesh, material.get() },
		mat_transform,
		layer,
		render_layer
	);
}

}  // namespace lili
