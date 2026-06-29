#include "lili2d/render/scene/shapes/rect.hpp"

#include "lili2d/geometry/utils.hpp"
#include "lili2d/geometry/vec3.hpp"

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
	if (shape.w != size.x || shape.h != size.y) {
		shape.w = size.x;
		shape.h = size.y;
		hollow_dirty = true;
	}
}

void Rect::setShape(RectShape shape) {
	if (this->shape.w != shape.w || this->shape.h != shape.h) {
		hollow_dirty = true;
	}
	this->shape = shape;
}

void Rect::setColor(Vec4 color) {
	material->properties.color_tint = color;
}

void Rect::setHollow(bool hollow) {
	is_hollow = hollow;
}

void Rect::setHollowThickness(float thickness) {
	if (hollow_thickness != thickness) {
		hollow_thickness = thickness;
		hollow_dirty = true;
	}
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
		if (hollow_dirty) {
			float t = hollow_thickness;
			float w = shape.w;
			float h = shape.h;

			std::vector<Vertex> vertices;
			std::vector<uint32_t> indices;

			auto add_quad = [&](float qx, float qy, float qw, float qh) {
				uint32_t start_idx = vertices.size();
				vertices.push_back(Vertex{qx, qy, 0.0f, 0.0f, 0.0f});
				vertices.push_back(Vertex{qx + qw, qy, 0.0f, 0.0f, 0.0f});
				vertices.push_back(Vertex{qx, qy + qh, 0.0f, 0.0f, 0.0f});
				vertices.push_back(Vertex{qx + qw, qy + qh, 0.0f, 0.0f, 0.0f});

				indices.push_back(start_idx + 0);
				indices.push_back(start_idx + 1);
				indices.push_back(start_idx + 2);
				indices.push_back(start_idx + 2);
				indices.push_back(start_idx + 1);
				indices.push_back(start_idx + 3);
			};

			float inner_h = h - 2.0f * t;
			// Top
			add_quad(0.0f, 0.0f, w, t);
			// Bottom
			add_quad(0.0f, h - t, w, t);
			if (inner_h > 0.0f) {
				// Left
				add_quad(0.0f, t, t, inner_h);
				// Right
				add_quad(w - t, t, t, inner_h);
			}

			MeshData mesh_data;
			mesh_data.vertices = vertices;
			mesh_data.indices = indices;
			if (!hollow_mesh) {
				hollow_mesh = std::make_unique<GPUMesh>(
					renderer->getDevice(), mesh_data
				);
			} else {
				hollow_mesh->update(mesh_data);
			}
			hollow_dirty = false;
		}

		Mat3 mat_transform = (
			Mat3::translate({ shape.x, shape.y }) * Mat3::rotation(rotation)
		);
		renderer->submit(
			Model(hollow_mesh.get(), material.get()),
			mat_transform,
			layer,
			render_layer
		);
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
