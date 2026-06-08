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

/**
 * \brief Defines the UV coordinates for a single glyph in a bitmap font.
 */
struct GlyphUV {
	float u0, v0, u1, v1 = 0; ///< UV coordinates.
};

/**
 * \brief Represents a bitmap font loaded from an image.
 */
class BitmapFont {
public:
	/**
	 * \brief Constructs a bitmap font.
	 * \param renderer The renderer.
	 * \param path Path to the font image.
	 * \param cols Number of columns in the font grid.
	 * \param rows Number of rows in the font grid.
	 */
	BitmapFont(
		Renderer *renderer, const std::string &path, uint8_t cols, uint8_t rows
	);
	/// \brief Default destructor.
	~BitmapFont() = default;

	/**
	 * \brief Gets the font's underlying texture.
	 * \return Pointer to the texture.
	 */
	Texture *get_texture() const;
	/**
	 * \brief Gets the width of a single glyph.
	 * \return The width in pixels.
	 */
	int get_glyph_w() const;
	/**
	 * \brief Gets the height of a single glyph.
	 * \return The height in pixels.
	 */
	int get_glyph_h() const;
	/**
	 * \brief Calculates the UV coordinates for a specific character.
	 * \param c The character.
	 * \return The UV coordinates.
	 */
	GlyphUV glyph_uv(char c) const;

private:
	std::unique_ptr<Texture> texture = nullptr;
	int cols = 1;
	int rows = 1;

	int glyph_w = 1;
	int glyph_h = 1;
};

/**
 * \brief Represents renderable text using a bitmap font.
 */
class Text {
public:
	/// \brief Default constructor.
	Text() = default;
	/**
	 * \brief Constructs text using a bitmap font.
	 * \param renderer The renderer.
	 * \param font Pointer to the bitmap font.
	 * \param text The text string to render.
	 */
	Text(Renderer *renderer, BitmapFont *font, const std::string &text);

	/// \brief Move constructor.
	Text(Text&&) = default;
	/**
	 * \brief Move assignment operator.
	 * \return Reference to the assigned text.
	 */
	Text& operator=(Text&&) = default;

	/**
	 * \brief Sets the text string.
	 * \param value The new text string.
	 */
	void set_text(const std::string &value);
	/**
	 * \brief Sets the text's position.
	 * \param position The new position.
	 */
	void set_position(const Vec2 &position);
	/**
	 * \brief Sets the spacing between characters.
	 * \param value The spacing value.
	 */
	void set_spacing(float value);
	/**
	 * \brief Sets the text's scale.
	 * \param value The new scale.
	 */
	void set_scale(float value);
	/**
	 * \brief Sets the depth value for Z-ordering.
	 * 
	 * This determines the drawing order relative to other objects within the same render pass.
	 * To change which render pass this object belongs to, use set_render().
	 * 
	 * \param layer The new layer depth.
	 */
	void set_layer(float layer);
	/**
	 * \brief Sets the render pass layer.
	 * 
	 * This determines which overall pass (e.g., WORLD2D or UI) the object is drawn in.
	 * To change the depth ordering within a pass, use set_layer().
	 * 
	 * \param render_layer The new render pass layer.
	 */
	void set_render(RenderLayer render_layer);

	/**
	 * \brief Gets the material.
	 * \return Pointer to the material.
	 */
	Material* get_material() const;

	/// \brief Submits the text for drawing.
	void draw();

private:
	Renderer *renderer = nullptr;
	BitmapFont *font = nullptr;

	std::string text = "default";
	Vec2 pos;

	float scale = 1.0f;
	float glyph_w = 1.0f;
	float glyph_h = 1.0f;
	float advance = 1.0f;

	std::unique_ptr<Material> material = nullptr;
	std::unique_ptr<GPUMesh> mesh = nullptr;
	Model model;
	float layer = 0.0f;

	RenderLayer render_layer = RenderLayer::WORLD2D;

	/// \brief rebuild_mesh method.
	void rebuild_mesh();
};

}  // namespace lili
