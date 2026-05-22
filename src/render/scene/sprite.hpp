#pragma once

#include <SDL3/SDL_gpu.h>
#include <string>
#include <memory>

#include "render/renderer.hpp"
#include "render/core/texture.hpp"
#include "render/scene/material.hpp"
#include "geometry/vec2.hpp"

namespace lili {

MeshData create_unit_quad();

class Sprite {
public:
	Sprite();
	~Sprite() = default;

	void set_texture(Renderer *renderer, const std::string &path);
	void set_position(Vec2 position);
	void set_scale(Vec2 scale);
	void set_rotation(float rotation);
	void set_layer(float layer);
	void draw(Renderer *renderer);

private:
	std::unique_ptr<Texture> texture;
	std::unique_ptr<Material> material;
	std::unique_ptr<GPUMesh> mesh;
	Model model;

	Vec2 position;
	Vec2 scale;
	float rotation;
	float layer;
};

}  // namespace lili
