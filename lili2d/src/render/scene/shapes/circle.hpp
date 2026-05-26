#pragma once

#include <memory>

#include "geometry/vec4.hpp"
#include "geometry/vec2.hpp"
#include "render/renderer.hpp"
#include "render/core/gpu_mesh.hpp"
#include "render/scene/material.hpp"

namespace lili {

struct CircleShape {
	Vec2 center;
	float radius;
	int segments;

	CircleShape() = default;
	CircleShape(const CircleShape &) = default;
	CircleShape(Vec2 center, float radius, int segments);
};

class Circle {
public:
	Circle() = default;
	Circle(Renderer *renderer, CircleShape shape, Vec4 color);
	~Circle() = default;

	Circle(Circle&&) = default;
	Circle& operator=(Circle&&) = default;

	void set_center(Vec2 pos);
	void set_radius(float r);
	void set_segments(int n);
	void set_shape(CircleShape shape);
	void set_color(Vec4 color);
	void set_layer(float value);

	Vec2 get_center() const;
	float get_radius() const;
	CircleShape get_shape() const;
	Vec4 get_color() const;

	void draw();

private:
	Renderer *renderer = nullptr;
	CircleShape shape{};

	float layer = 0.0f;
	std::unique_ptr<GPUMesh> mesh;
	std::unique_ptr<Material> material;
};

}  // namespace lili
