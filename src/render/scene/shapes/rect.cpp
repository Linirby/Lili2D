#include "lili2d/render/scene/shapes/rect.hpp"

#include "lili2d/geometry/utils.hpp"

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

void Rect::setHollow(bool hollow) {
	is_hollow = hollow;
}

void Rect::setHollowThickness(float thickness) {
	hollow_thickness = thickness;
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

bool Rect::isHollow() const {
	return is_hollow;
}

float Rect::getHollowThickness() const {
	return hollow_thickness;
}

void Rect::draw() {
	if (is_hollow) {
		Mat3 mat_rect = (
			Mat3::translate({ shape.x, shape.y }) * Mat3::rotation(rotation)
		);
		float inner_h = shape.h - 2.0f * hollow_thickness;

		Mat3 top_mat = (
			mat_rect *
			Mat3::translate({ 0.0f, 0.0f }) *
			Mat3::scale({ shape.w, hollow_thickness })
		);
		renderer->submit(
			Model(mesh, material.get()), top_mat, layer, render_layer
		);

		Mat3 bottom_mat = (
			mat_rect *
			Mat3::translate({ 0.0f, shape.h - hollow_thickness }) *
			Mat3::scale({ shape.w, hollow_thickness })
		);
		renderer->submit(
			Model(mesh, material.get()), bottom_mat, layer, render_layer
		);

		if (inner_h > 0.0f) {
			Mat3 left_mat = (
				mat_rect *
				Mat3::translate({ 0.0f, hollow_thickness }) *
				Mat3::scale({ hollow_thickness, inner_h })
			);
			renderer->submit(
				Model(mesh, material.get()), left_mat, layer, render_layer
			);

			Mat3 right_mat = (
				mat_rect *
				Mat3::translate({
					shape.w - hollow_thickness,
					hollow_thickness
				}) *
				Mat3::scale({ hollow_thickness, inner_h })
			);
			renderer->submit(
				Model(mesh, material.get()), right_mat, layer, render_layer
			);
		}
	} else {
		Mat3 mat_transform = (
			Mat3::translate({ shape.x, shape.y }) *
			Mat3::rotation(rotation) *
			Mat3::scale({ shape.w, shape.h })
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
