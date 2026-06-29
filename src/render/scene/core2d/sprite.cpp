#include "lili2d/render/scene/core2d/sprite.hpp"

#include "lili2d/render/scene/common/utils.hpp"
#include "lili2d/geometry/mat3x3.hpp"
#include "lili2d/geometry/utils.hpp"

namespace lili {

Sprite::Sprite(Renderer *renderer, const std::string &path)
	: renderer(renderer) {
	texture = std::make_unique<Texture>(renderer->getDevice(), path);
	mesh = renderer->getUnitQuad();

	material = std::make_unique<Material>(texture.get());
	material->properties.color_tint = { 1.0f, 1.0f, 1.0f, 1.0f };

	position = { 0.0f, 0.0f };
	scale = { 1.0f, 1.0f };
	size = { (float)texture->getWidth(), (float)texture->getHeight() };
	rotation = 0.0f;
	layer = 0.0f;
}

Sprite::Sprite(Renderer *renderer, SliceUV slice)
	: renderer(renderer) {
	mesh = renderer->getUnitQuad();

	material = std::make_unique<Material>(slice.texture);
	material->properties.color_tint = { 1.0f, 1.0f, 1.0f, 1.0f };

	position = { 0.0f, 0.0f };
	scale = { 1.0f, 1.0f };
	size = { slice.width, slice.height };
	rotation = 0.0f;
	layer = 0.0f;
	setSlice(slice);
}

void Sprite::setImage(const std::string &path) {
	texture = std::make_unique<Texture>(renderer->getDevice(), path);
	material = std::make_unique<Material>(texture.get());
	material->properties.color_tint = { 1.0f, 1.0f, 1.0f, 1.0f };
	mesh = renderer->getUnitQuad();
}

void Sprite::setColorTint(const Vec4 &color) {
	material->properties.color_tint = color;
}

void Sprite::setPosition(const Vec2 &position) {
	this->position = position;
}

void Sprite::setScale(const Vec2 &scale) {
	this->scale = scale;
}

void Sprite::setSize(const Vec2 &size) {
	this->size = size;
}

void Sprite::setRotation(float degree) {
	rotation = lili::degToRad(degree);
}

void Sprite::setLayer(float layer) {
	this->layer = layer;
}

void Sprite::setSlice(SliceUV slice) {
	current_slice = slice;
	if (material) {
		material->albedoMap = slice.texture;
		material->properties.uv_bounds = {
			slice.u_min, slice.v_min, slice.u_max, slice.v_max
		};
	}
}

Vec2 Sprite::getPosition() const {
	return position;
}

float Sprite::getWidth() const {
	return size.x * scale.x;
}

float Sprite::getHeight() const {
	return size.y * scale.y;
}

Vec2 Sprite::getSize() const {
	return Vec2(size.x * scale.x, size.y * scale.y);
}

Material* Sprite::getMaterial() const {
	return material.get();
}

void Sprite::draw() {
	Mat3 mat_transform = (
		Mat3::translate(position) *
		Mat3::rotation(rotation) *
		Mat3::scale(getSize())
	);
	renderer->submit(
		Model({ mesh, material.get() }),
		mat_transform,
		layer,
		RenderLayer::WORLD2D
	);
}

}  // namespace lili
