#include "lili2d/render/scene/core2d/sprite_batch.hpp"

#include "lili2d/geometry/mat3x3.hpp"
#include "lili2d/geometry/utils.hpp"
#include <cmath>

namespace lili {

SpriteBatch::SpriteBatch(Renderer *renderer, Texture *texture)
	: renderer(renderer) {
	material = std::make_unique<Material>(texture);
	material->properties.color_tint = { 1.0f, 1.0f, 1.0f, 1.0f };

	mesh = std::make_unique<GPUMesh>(renderer->getDevice(), mesh_data);

	position = { 0.0f, 0.0f };
	scale = { 1.0f, 1.0f };
	layer = 0.0f;
}

SpriteBatch::~SpriteBatch() {}

void SpriteBatch::begin() {
	mesh_data.vertices.clear();
	mesh_data.indices.clear();
}

void SpriteBatch::clear() {
	mesh_data.vertices.clear();
	mesh_data.indices.clear();
	mesh->update(mesh_data);
}

void SpriteBatch::appendSpriteToMesh(
	MeshData &mesh_data,
	const SliceUV &slice,
	const Vec2 &pos,
	const Vec2 &scale,
	float rotation,
	const Vec4 &color
) {
	uint32_t current_vertex_count = static_cast<uint32_t>(
		mesh_data.vertices.size()
	);

	float cos_r = 1.0f;
	float sin_r = 0.0f;

	if (rotation != 0.0f) {
		float rot_rad = lili::degToRad(rotation);
		cos_r = std::cos(rot_rad);
		sin_r = std::sin(rot_rad);
	}

	float half_w = (slice.width / 2.0f) * scale.x;
	float half_h = (slice.height / 2.0f) * scale.y;

	Vec2 p0{ -half_w, -half_h };
	Vec2 p1{  half_w, -half_h };
	Vec2 p2{  half_w,  half_h };
	Vec2 p3{ -half_w,  half_h };

	auto transform_point = [&](const Vec2 &p) -> Vec2 {
		return {
			pos.x + p.x * cos_r - p.y * sin_r,
			pos.y + p.x * sin_r + p.y * cos_r
		};
	};

	Vec2 tp0 = transform_point(p0);
	Vec2 tp1 = transform_point(p1);
	Vec2 tp2 = transform_point(p2);
	Vec2 tp3 = transform_point(p3);

	Vertex v0;
	v0.x = tp0.x; v0.y = tp0.y; v0.z = 0.0f;
	v0.u = slice.u_min; v0.v = slice.v_min;
	v0.material_id = 0.0f;
	v0.r = color.x; v0.g = color.y; v0.b = color.z; v0.a = color.w;

	Vertex v1;
	v1.x = tp1.x; v1.y = tp1.y; v1.z = 0.0f;
	v1.u = slice.u_max; v1.v = slice.v_min;
	v1.material_id = 0.0f;
	v1.r = color.x; v1.g = color.y; v1.b = color.z; v1.a = color.w;

	Vertex v2;
	v2.x = tp2.x; v2.y = tp2.y; v2.z = 0.0f;
	v2.u = slice.u_max; v2.v = slice.v_max;
	v2.material_id = 0.0f;
	v2.r = color.x; v2.g = color.y; v2.b = color.z; v2.a = color.w;

	Vertex v3;
	v3.x = tp3.x; v3.y = tp3.y; v3.z = 0.0f;
	v3.u = slice.u_min; v3.v = slice.v_max;
	v3.material_id = 0.0f;
	v3.r = color.x; v3.g = color.y; v3.b = color.z; v3.a = color.w;

	mesh_data.vertices.push_back(v0);
	mesh_data.vertices.push_back(v1);
	mesh_data.vertices.push_back(v2);
	mesh_data.vertices.push_back(v3);

	mesh_data.indices.push_back(current_vertex_count + 0);
	mesh_data.indices.push_back(current_vertex_count + 1);
	mesh_data.indices.push_back(current_vertex_count + 2);
	mesh_data.indices.push_back(current_vertex_count + 2);
	mesh_data.indices.push_back(current_vertex_count + 3);
	mesh_data.indices.push_back(current_vertex_count + 0);
}

void SpriteBatch::setMeshData(MeshData &&data) {
	mesh_data = std::move(data);
	mesh->update(mesh_data);
}

void SpriteBatch::draw(
	const SliceUV &slice,
	const Vec2 &pos,
	const Vec2 &scale,
	float rotation,
	const Vec4 &color
) {
	appendSpriteToMesh(mesh_data, slice, pos, scale, rotation, color);
}

void SpriteBatch::end() {
	mesh->update(mesh_data);
}

void SpriteBatch::setColorTint(const Vec4 &color) {
	material->properties.color_tint = color;
}

void SpriteBatch::setPosition(const Vec2 &position) {
	this->position = position;
}

void SpriteBatch::setScale(const Vec2 &scale) {
	this->scale = scale;
}

void SpriteBatch::setLayer(float layer) {
	this->layer = layer;
}

void SpriteBatch::draw() {
	if (mesh_data.indices.empty()) return;

	Mat3 mat_transform = Mat3::translate(position) * Mat3::scale(scale);

	renderer->submit(
		Model({ mesh.get(), material.get() }),
		mat_transform,
		layer,
		RenderLayer::WORLD2D
	);
}

}  // namespace lili
