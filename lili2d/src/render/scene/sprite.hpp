#pragma once

#include <SDL3/SDL_gpu.h>
#include <string>
#include <memory>

#include "render/renderer.hpp"
#include "render/core/texture.hpp"
#include "render/scene/material.hpp"
#include "geometry/vec2.hpp"

namespace lili {

/**
 * \brief Represents a renderable 2D sprite.
 */
class Sprite {
public:
	/// \brief Default constructor.
	Sprite() = default;
	/**
	 * \brief Constructs a sprite from an image file.
	 * \param renderer The renderer.
	 * \param path Path to the image file.
	 */
	Sprite(Renderer *renderer, const std::string &path);
	/// \brief Default destructor.
	~Sprite() = default;

	/// \brief Move constructor.
	Sprite(Sprite&&) = default;
	/**
	 * \brief Move assignment operator.
	 * \return Reference to the assigned sprite.
	 */
	Sprite& operator=(Sprite&&) = default;

	/**
	 * \brief Sets the sprite's image.
	 * \param path Path to the new image file.
	 */
	void set_image(const std::string &path);
	/**
	 * \brief Sets the sprite's color tint.
	 * \param color The new color tint.
	 */
	void set_color_tint(const Vec4 &color);
	/**
	 * \brief Sets the sprite's position.
	 * \param position The new position.
	 */
	void set_position(const Vec2 &position);
	/**
	 * \brief Sets the sprite's scale.
	 * \param scale The new scale.
	 */
	void set_scale(const Vec2 &scale);
	/**
	 * \brief Sets the sprite's size (in px).
	 * \param size The new size.
	 */
	void set_size(const Vec2 &size);
	/**
	 * \brief Sets the sprite's rotation.
	 * \param degree The rotation in degrees.
	 */
	void set_rotation(float degree);
	/**
	 * \brief Sets the sprite's rendering layer.
	 * \param layer The new layer depth.
	 */
	void set_layer(float layer);

	/**
	 * \brief Get the render width of the sprite.
	 * \return The width.
	 */
	float get_width() const;
	/**
	 * \brief Get the render height of the sprite.
	 * \return The height.
	 */
	float get_height() const;
	/**
	 * \brief Get the render size of the sprite.
	 * \return A 2D vector of the width and height.
	 */
	Vec2 get_size() const;

	/// \brief Submits the sprite for drawing.
	void draw();

private:
	Renderer *renderer = nullptr;

	std::unique_ptr<Texture> texture = nullptr;
	std::unique_ptr<GPUMesh> mesh = nullptr;
	std::unique_ptr<Material> material = nullptr;

	Vec2 position;
	Vec2 scale = { 1, 1 };
	Vec2 size = { 1, 1 };
	float rotation = 0;
	float layer = 0;
};

}  // namespace lili
