#pragma once

#include <memory>

#include "geometry/vec4.hpp"
#include "geometry/vec2.hpp"
#include "render/renderer.hpp"
#include "render/core/gpu_mesh.hpp"
#include "render/scene/material.hpp"

namespace lili {

struct LineShape {
	Vec2 start;
	Vec2 end;
	float thickness;

	LineShape() = default;
	LineShape(const LineShape &) = default;
	LineShape(Vec2 start, Vec2 end, float thickness);
};

class Line {
public:
	Line() = default;
	Line(Renderer *renderer, LineShape shape, Vec4 color);
	~Line() = default;

	Line(Line&&) = default;
	Line& operator=(Line&&) = default;

	void set_start(Vec2 pos);
	void set_end(Vec2 pos);
	void set_thickness(float value);
	void set_shape(LineShape shape);
	void set_color(Vec4 color);
	void set_layer(float value);

	Vec2 get_start() const;
	Vec2 get_end() const;
	Vec2 get_thickness() const;
	LineShape get_shape() const;
	Vec4 get_color() const;

	void draw();

private:
	Renderer *renderer = nullptr;
	LineShape shape{};

	float layer = 0.0f;
	std::unique_ptr<GPUMesh> mesh;
	std::unique_ptr<Material> material;
};

}  // namespace lili
