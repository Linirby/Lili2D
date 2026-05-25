#pragma once

#include <SDL3/SDL_gpu.h>
#include <string>
#include <memory>

#include "render/renderer.hpp"
#include "render/core/texture.hpp"
#include "render/scene/material.hpp"
#include "geometry/vec2.hpp"

namespace lili {

class Sprite {
public:
	Sprite(Renderer *renderer, const std::string &path);
	~Sprite() = default;

	void set_image(const std::string &path);
	void set_color_tint(const Vec4 &color);
	void set_position(const Vec2 &position);
	void set_scale(const Vec2 &scale);
	void set_rotation(float degree);
	void set_layer(float layer);
	void draw();

private:
	Renderer *renderer;

	std::unique_ptr<Texture> texture;
	std::unique_ptr<GPUMesh> mesh;
	std::unique_ptr<Material> material;

	Vec2 position;
	Vec2 scale;
	float rotation;
	float layer;
};

}  // namespace lili
