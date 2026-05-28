#include "render/scene/shapes/circle.hpp"

#include "render/scene/utils.hpp"

namespace lili {

CircleShape::CircleShape(Vec2 center, float radius, int segments)
	: center(center), radius(radius), segments(segments) {}

Circle::Circle(Renderer *renderer, CircleShape shape, Vec4 color) {
	this->renderer = renderer;

	// Create circle with 32 segments
	mesh = renderer->get_unit_circle(shape.segments);
	material = std::make_unique<Material>(renderer->get_the_white_pixel());

	set_shape(shape);
	set_color(color);

	layer = 0.0f;
}

void Circle::set_center(Vec2 pos) {
	shape.center = pos;
}

void Circle::set_radius(float r) {
	shape.radius = r;
}

void Circle::set_segments(int n) {
	shape.segments = n;
	mesh = renderer->get_unit_circle(n);
}

void Circle::set_shape(CircleShape shape) {
	this->shape = shape;
}

void Circle::set_color(Vec4 color) {
	material->properties.color_tint = color;
}

void Circle::set_layer(float value) {
	layer = value;
}

void Circle::set_render(RenderLayer render_layer) {
	this->render_layer = render_layer;
}

Vec2 Circle::get_center() const {
	return shape.center;
}

float Circle::get_radius() const {
	return shape.radius;
}

CircleShape Circle::get_shape() const {
	return shape;
}

Vec4 Circle::get_color() const {
	return material->properties.color_tint;
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
