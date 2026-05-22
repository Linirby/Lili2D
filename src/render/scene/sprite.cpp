#include "render/scene/sprite.hpp"
#include "geometry/mat3x3.hpp"

namespace lili {

MeshData create_unit_quad() {
	MeshData quad;

	quad.vertices = {
		(Vertex){
			.x = -0.5f, .y = -0.5f, .z = 0.0f,
			.u = 0.0f, .v = 0.0f,
			.material_id = 0
		},
		(Vertex){
			.x = 0.5f, .y = -0.5f, .z = 0.0f,
			.u = 1.0f, .v = 0.0f,
			.material_id = 0
		},
		(Vertex){
			.x = 0.5f, .y = 0.5f, .z = 0.0f,
			.u = 1.0f, .v = 1.0f,
			.material_id = 0
		},
		(Vertex){
			.x = -0.5f, .y = 0.5f, .z = 0.0f,
			.u = 0.0f, .v = 1.0f,
			.material_id = 0
		}
	};
	quad.indices = {
		0, 1, 2,
		2, 3, 0
	};
	return quad;
}


Sprite::Sprite() {
	texture = nullptr;
	material = nullptr;
	mesh = nullptr;
	model = {};
	position = { 0.0f, 0.0f };
	scale = { 0.0f, 0.0f };
	rotation = 0.0f;
	layer = 0.0f;
}

void Sprite::set_texture(Renderer *renderer, const std::string &path) {
	texture = std::make_unique<Texture>(renderer->get_device(), path);
	material = std::make_unique<Material>(texture.get());
	material->properties.color_tint = { 1.0f, 1.0f, 1.0f, 1.0f };

	MeshData mesh_data = create_unit_quad();
	mesh = std::make_unique<GPUMesh>(renderer->get_device(), mesh_data);
	model = Model(mesh.get(), material.get());
}

void Sprite::set_position(Vec2 position) {
	this->position = position;
}

void Sprite::set_scale(Vec2 scale) {
	this->scale = scale;
}

void Sprite::set_rotation(float rotation) {
	this->rotation = rotation;
}

void Sprite::set_layer(float layer) {
	this->layer = layer;
}

void Sprite::draw(Renderer *renderer) {
	Mat3 mat_transform = (
		Mat3::translate(position) *
		Mat3::rotation(rotation) *
		Mat3::scale(scale)
	);
	renderer->submit(model, mat_transform, layer, RenderLayer::UI2D);
}

}  // namespace lili
