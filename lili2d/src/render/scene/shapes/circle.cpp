#include "circle.hpp"

#include <cmath>

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

void Circle::setHollow(bool hollow) {
	is_hollow = hollow;
}

void Circle::setHollowThickness(float thickness) {
	hollow_thickness = thickness;
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

Vec2 Circle::getTopLeft() const {
	return shape.center - Vec2(shape.radius, shape.radius);
}

float Circle::getRadius() const {
	return shape.radius;
}

float Circle::getDiameter() const {
	return shape.radius * 2;
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

bool Circle::isHollow() const {
	return is_hollow;
}

float Circle::getHollowThickness() const {
	return hollow_thickness;
}

void Circle::draw() {
	if (is_hollow) {
		int n = shape.segments;
		float r = shape.radius;
		const float PI = 3.14159265359f;
		float angle_step = (2.0f * PI) / n;

		for (int i = 0; i < n; i++) {
			float angle1 = i * angle_step;
			float angle2 = (i + 1) * angle_step;

			Vec2 p1 = (
				shape.center + Vec2(std::cos(angle1) * r, std::sin(angle1) * r)
			);
			Vec2 p2 = (
				shape.center + Vec2(std::cos(angle2) * r, std::sin(angle2) * r)
			);

			Vec2 diff = p2 - p1;
			float length = diff.length();
			float angle = std::atan2(diff.y, diff.x);

			Mat3 mat_transform = (
				Mat3::translate(p1) *
				Mat3::rotation(angle) *
				Mat3::translate({ 0.0f, -hollow_thickness * 0.5f }) *
				Mat3::scale({ length, hollow_thickness })
			);

			renderer->submit(
				Model(renderer->getUnitQuad(), material.get()),
				mat_transform,
				layer,
				render_layer
			);
		}
	} else {
		Mat3 mat_transform = (
			Mat3::translate(shape.center) *
			Mat3::scale({ shape.radius * 2.0f, shape.radius * 2.0f })
		);
		renderer->submit(
			Model(mesh, material.get()),
			mat_transform,
			layer,
			render_layer
		);
	}
}

}  // namespace lili
