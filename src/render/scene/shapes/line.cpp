#include "lili2d/render/scene/shapes/line.hpp"

#include <cmath>

namespace lili {

LineShape::LineShape(Vec2 start, Vec2 end, float thickness)
	: start(start), end(end), thickness(thickness) {}

Line::Line(Renderer *renderer, LineShape shape, Vec4 color)
	: renderer(renderer) {
	mesh = renderer->getUnitQuad();
	material = std::make_unique<Material>(renderer->getTheWhitePixel());
	setShape(shape);
	setColor(color);
	layer = 0.0f;
}

void Line::setStart(Vec2 pos) {
	shape.start = pos;
}

void Line::setEnd(Vec2 pos) {
	shape.end = pos;
}

void Line::setThickness(float value) {
	shape.thickness = value;
}

void Line::setShape(LineShape shape) {
	this->shape = shape;
}

void Line::setColor(Vec4 color) {
	material->properties.color_tint = color;
}

void Line::setLayer(float value) {
	layer = value;
}

void Line::setRender(RenderLayer render_layer) {
	this->render_layer = render_layer;
}

Vec2 Line::getStart() const {
	return shape.start;
}

Vec2 Line::getEnd() const {
	return shape.end;
}

float Line::getThickness() const {
	return shape.thickness;
}

LineShape Line::getShape() const {
	return shape;
}

Vec4 Line::getColor() const {
	return material->properties.color_tint;
}

Material* Line::getMaterial() const {
	return material.get();
}

void Line::draw() {
	Vec2 diff = shape.end - shape.start;
	float length = diff.length();
	float angle = std::atan2(diff.y, diff.x);

	Mat3 mat_transform = (
		Mat3::translate(shape.start) *
		Mat3::rotation(angle) *
		Mat3::translate({ 0.0f, -shape.thickness * 0.5f }) *
		Mat3::scale({ length, shape.thickness })
	);
	renderer->submit(
		(Model){ mesh, material.get() },
		mat_transform,
		layer,
		render_layer
	);
}

}  // namespace lili
