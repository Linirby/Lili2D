#include "render/scene/shapes/circle.hpp"

namespace lili {

CircleShape::CircleShape(Vec2 center, float radius, int segments)
	: center(center), radius(radius), segments(segments) {}

Circle::Circle(Renderer *renderer, CircleShape shape, Vec4 color)
	: renderer(renderer) {
	mesh = renderer->getUnitCircle(shape.segments);
	material = std::make_unique<Material>(renderer->getTheWhitePixel());
	setShape(shape);
	setColor(color);
	layer = 0.0f;
}

void Circle::setCenter(Vec2 pos) {
	shape.center = pos;
}

void Circle::setRadius(float r) {
	shape.radius = r;
}

void Circle::setSegments(int n) {
	shape.segments = n;
	mesh = renderer->getUnitCircle(n);
}

void Circle::setShape(CircleShape shape) {
	this->shape = shape;
}

void Circle::setColor(Vec4 color) {
	material->properties.color_tint = color;
}

void Circle::setLayer(float value) {
	layer = value;
}

void Circle::setRender(RenderLayer render_layer) {
	this->render_layer = render_layer;
}

Vec2 Circle::getCenter() const {
	return shape.center;
}

float Circle::getRadius() const {
	return shape.radius;
}

CircleShape Circle::getShape() const {
	return shape;
}

Vec4 Circle::getColor() const {
	return material->properties.color_tint;
}

Material* Circle::getMaterial() const {
	return material.get();
}

void Circle::draw() {
	Mat3 mat_transform = (
		Mat3::translate(shape.center) *
		Mat3::scale({ shape.radius * 2.0f, shape.radius * 2.0f })
	);
	renderer->submit(
		(Model){ mesh, material.get() },
		mat_transform,
		layer,
		render_layer
	);
}

}  // namespace lili
