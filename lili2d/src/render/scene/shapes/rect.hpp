#pragma once

#include <memory>

#include "geometry/vec4.hpp"
#include "geometry/vec2.hpp"
#include "render/renderer.hpp"
#include "render/core/gpu_mesh.hpp"
#include "render/scene/material.hpp"

namespace lili {

struct RectShape {
	float x, y, w, h;

	RectShape() = default;
	RectShape(const RectShape &) = default;
	RectShape(float x, float y, float w, float h);
};

class Rect {
public:
	Rect() = default;
	Rect(Renderer *renderer, RectShape shape, Vec4 color);
	~Rect() = default;

	Rect(Rect&&) = default;
	Rect& operator=(Rect&&) = default;

	void set_position(Vec2 pos);
	void set_size(Vec2 size);
	void set_rotation(float degree);
	void set_shape(RectShape shape);
	void set_color(Vec4 color);
	void set_layer(float value);

	Vec2 get_position() const;
	Vec2 get_size() const;
	RectShape get_shape() const;
	Vec4 get_color() const;

	void draw();

private:
	Renderer *renderer = nullptr;
	RectShape shape{};

	float rotation = 0.0f;
	float layer = 0.0f;
	std::unique_ptr<GPUMesh> mesh;
	std::unique_ptr<Material> material;
};

}  // namespace lili
