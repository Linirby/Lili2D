#pragma once

#include <SDL3/SDL_gpu.h>
#include <string>
#include <memory>

#include "render/core/texture.hpp"
#include "render/scene/material.hpp"
#include "render/core/gpu_mesh.hpp"
#include "render/scene/model.hpp"
#include "geometry/vec2.hpp"
#include "render/renderer.hpp"

namespace lili {

struct GlyphUV {
	float u0, v0, u1, v1;
};

class BitmapFont {
public:
	BitmapFont(
		Renderer *renderer, const std::string &path, uint8_t cols, uint8_t rows
	);
	~BitmapFont() = default;

	Texture *get_texture() const;
	int get_glyph_w() const;
	int get_glyph_h() const;
	GlyphUV glyph_uv(char c) const;

private:
	std::unique_ptr<Texture> texture;
	int cols;
	int rows;

	int glyph_w;
	int glyph_h;
};

class Text {
public:
	Text(Renderer *renderer, BitmapFont *font, const std::string &text);

	void set_text(const std::string &value);
	void set_position(const Vec2 &position);
	void set_spacing(float value);
	void set_scale(float value);
	void set_layer(float layer);
	void draw();

private:
	Renderer *renderer = nullptr;
	BitmapFont *font = nullptr;

	std::string text;
	Vec2 pos;

	float scale = 1.0f;
	float glyph_w = 1.0f;
	float glyph_h = 1.0f;
	float advance = 1.0f;

	std::unique_ptr<Material> material;
	std::unique_ptr<GPUMesh> mesh;
	Model model;
	float layer = 0.0f;

	void rebuild_mesh();
};

}  // namespace lili
