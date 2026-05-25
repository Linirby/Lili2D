#include "render/scene/sprite.hpp"

#include "render/scene/utils.hpp"
#include "geometry/mat3x3.hpp"
#include "geometry/utils.hpp"

namespace lili {

Sprite::Sprite(Renderer *renderer, const std::string &path) {
	this->renderer = renderer;
	texture = std::make_unique<Texture>(renderer->get_device(), path);

	MeshData mesh_data = create_unit_quad();
	mesh = std::make_unique<GPUMesh>(renderer->get_device(), mesh_data);

	material = std::make_unique<Material>(texture.get());
	material->properties.color_tint = { 1.0f, 1.0f, 1.0f, 1.0f };

	position = { 0.0f, 0.0f };
	scale = { 0.0f, 0.0f };
	rotation = 0.0f;
	layer = 0.0f;
}

void Sprite::set_image(const std::string &path) {
	texture = std::make_unique<Texture>(renderer->get_device(), path);
	material = std::make_unique<Material>(texture.get());
	material->properties.color_tint = { 1.0f, 1.0f, 1.0f, 1.0f };

	MeshData mesh_data = create_unit_quad();
	mesh = std::make_unique<GPUMesh>(renderer->get_device(), mesh_data);
}

void Sprite::set_color_tint(const Vec4 &color) {
	material->properties.color_tint = color;
}

void Sprite::set_position(const Vec2 &position) {
	this->position = position;
}

void Sprite::set_scale(const Vec2 &scale) {
	this->scale = scale;
}

void Sprite::set_rotation(float degree) {
	rotation = lili::deg_to_rad(degree);
}

void Sprite::set_layer(float layer) {
	this->layer = layer;
}

void Sprite::draw() {
	Mat3 mat_transform = (
		Mat3::translate(position) *
		Mat3::rotation(rotation) *
		Mat3::scale({
			scale.x * texture->get_width(),
			scale.y * texture->get_height()
		})
	);
	renderer->submit(
		(Model){ mesh.get(), material.get() },
		mat_transform,
		layer,
		RenderLayer::WORLD2D
	);
}

}  // namespace lili
