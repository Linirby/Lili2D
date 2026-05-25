#include "render/scene/shapes/rect.hpp"

#include "geometry/utils.hpp"
#include "render/scene/utils.hpp"

namespace lili {

RectShape::RectShape(float x, float y, float w, float h)
	: x(x), y(y), w(w), h(h) {}

Rect::Rect(Renderer *renderer, RectShape shape, Vec4 color) {
	this->renderer = renderer;

	MeshData mesh_data = create_unit_quad();
	mesh = std::make_unique<GPUMesh>(renderer->get_device(), mesh_data);
	material = std::make_unique<Material>(renderer->get_the_white_pixel());

	set_shape(shape);
	set_color(color);

	rotation = 0.0f;
	layer = 0.0f;
}

void Rect::set_position(Vec2 pos) {
	shape.x = pos.x;
	shape.y = pos.y;
}

void Rect::set_rotation(float degree) {
	rotation = lili::deg_to_rad(degree);
}

void Rect::set_size(Vec2 size) {
	shape.w = size.x;
	shape.h = size.y;
}

void Rect::set_shape(RectShape shape) {
	this->shape = shape;
}

void Rect::set_color(Vec4 color) {
	material->properties.color_tint = color;
}

void Rect::set_layer(float value) {
	layer = value;
}

Vec2 Rect::get_position() const {
	return { shape.x, shape.y };
}

Vec2 Rect::get_size() const {
	return { shape.w, shape.h };
}

RectShape Rect::get_shape() const {
	return shape;
}

Vec4 Rect::get_color() const {
	return material->properties.color_tint;
}

void Rect::draw() {
	Mat3 mat_transform = (
		Mat3::translate({ shape.x, shape.y }) *
		Mat3::rotation(rotation) *
		Mat3::translate({ shape.w * 0.5f, shape.h * 0.5f }) *
		Mat3::scale({ shape.w, shape.h })
	);
	renderer->submit(
		(Model){ mesh.get(), material.get() },
		mat_transform,
		layer,
		RenderLayer::WORLD2D
	);
}

}  // namespace lili
