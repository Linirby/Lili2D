#pragma once

#include <SDL3/SDL_gpu.h>
#include <string>
#include <memory>

#include "render/renderer.hpp"
#include "render/core/texture.hpp"
#include "render/scene/material.hpp"

namespace lili {

MeshData create_unit_quad();

class Sprite {
public:
	Sprite();
	~Sprite() = default;

	void set_texture(Renderer *renderer, const std::string &path);
	void set_position(Vec3 position);
	void set_scale(Vec3 scale);
	void set_rotation(Vec3 rotation);
	void draw(Renderer *renderer);

private:
	std::unique_ptr<Texture> texture;
	std::unique_ptr<Material> material;
	std::unique_ptr<GPUMesh> mesh;
	Model model;

	Vec3 position;
	Vec3 scale;
	Vec3 rotation;
};

}  // namespace lili
