#include "render/scene/shapes/line.hpp"

#include <cmath>
#include "render/scene/utils.hpp"

namespace lili {

LineShape::LineShape(Vec2 start, Vec2 end, float thickness)
	: start(start), end(end), thickness(thickness) {}

Line::Line(Renderer *renderer, LineShape shape, Vec4 color) {
	this->renderer = renderer;

	MeshData mesh_data = create_unit_quad();
	mesh = std::make_unique<GPUMesh>(renderer->get_device(), mesh_data);
	material = std::make_unique<Material>(renderer->get_the_white_pixel());

	set_shape(shape);
	set_color(color);

	layer = 0.0f;
}

void Line::set_start(Vec2 pos) {
	shape.start = pos;
}

void Line::set_end(Vec2 pos) {
	shape.end = pos;
}

void Line::set_thickness(float value) {
	shape.thickness = value;
}

void Line::set_shape(LineShape shape) {
	this->shape = shape;
}

void Line::set_color(Vec4 color) {
	material->properties.color_tint = color;
}

void Line::set_layer(float value) {
	layer = value;
}

Vec2 Line::get_start() const {
	return shape.start;
}

Vec2 Line::get_end() const {
	return shape.end;
}

LineShape Line::get_shape() const {
	return shape;
}

Vec4 Line::get_color() const {
	return material->properties.color_tint;
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
		(Model){ mesh.get(), material.get() },
		mat_transform,
		layer,
		RenderLayer::WORLD2D
	);
}

}  // namespace lili
