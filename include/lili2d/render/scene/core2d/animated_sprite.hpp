#pragma once

#include <cstdint>
#include <memory>
#include <string>

#include "lili2d/render/renderer.hpp"
#include "lili2d/render/scene/common/material.hpp"
#include "lili2d/render/scene/common/animation.hpp"
#include "lili2d/geometry/vec2.hpp"

namespace lili {

/// @brief A renderable 2D sprite driven by an Animation.
///
/// AnimatedSprite takes an Animation (an image optionally sliced into
/// frames) and plays through them over time. Call update() each frame
/// to advance the animation, and draw() to render the current frame.
class AnimatedSprite {
public:
	/// @brief Default constructor.
	AnimatedSprite() = default;
	/// @brief Constructs an animated sprite from an animation.
	/// @param renderer The renderer.
	/// @param animation The animation to play.
	AnimatedSprite(Renderer *renderer, const Animation &animation);
	/// @brief Constructs an animated sprite from a registered animation key.
	/// @param renderer The renderer.
	/// @param animation_key The registry key of the animation.
	AnimatedSprite(Renderer *renderer, const std::string &animation_key);
	/// @brief Constructs an animated sprite from a registered animation ID.
	/// @param renderer The renderer.
	/// @param animation_id The registry ID of the animation.
	AnimatedSprite(Renderer *renderer, uint16_t animation_id);
	/// @brief Move constructor.
	AnimatedSprite(AnimatedSprite&&) = default;
	/// @brief Move assignment operator.
	/// @return Reference to the assigned animated sprite.
	AnimatedSprite& operator=(AnimatedSprite&&) = default;

	/// @brief Sets the animation to play.
	/// @param animation The new animation.
	void setAnimation(const Animation &animation);
	/// @brief Sets the animation to play from a registry key.
	/// @param animation_key The registry key of the animation.
	void setAnimation(const std::string &animation_key);
	/// @brief Sets the animation to play from a registry ID.
	/// @param animation_id The registry ID of the animation.
	void setAnimation(uint16_t animation_id);

	/// @brief Sets the speed of the animation.
	/// @param speed_sec Time per frame in seconds.
	void setFrameSpeed(float speed_sec);

	/// @brief Sets the sprite's color tint.
	/// @param color The new color tint.
	void setColorTint(const Vec4 &color);
	/// @brief Sets the sprite's position.
	/// @param position The new position.
	void setPosition(const Vec2 &position);
	/// @brief Sets the sprite's scale.
	/// @param scale The new scale.
	void setScale(const Vec2 &scale);
	/// @brief Sets the sprite's size (in px).
	/// @param size The new size.
	void setSize(const Vec2 &size);
	/// @brief Sets the sprite's rotation.
	/// @param degree The rotation in degrees.
	void setRotation(float degree);
	/// @brief Sets the sprite's rendering layer.
	/// @param layer The new layer depth.
	void setLayer(float layer);

	/// @brief Get the position of the current frame.
	/// @return The position.
	Vec2 getPosition() const;
	/// @brief Get the render width of the current frame.
	/// @return The width.
	float getWidth() const;
	/// @brief Get the render height of the current frame.
	/// @return The height.
	float getHeight() const;
	/// @brief Get the render size of the current frame.
	/// @return A 2D vector of the width and height.
	Vec2 getSize() const;
	/// @brief Gets the material.
	/// @return Pointer to the material.
	Material* getMaterial() const;

	/// @brief Advances the animation by dt seconds.
	/// @param dt Delta time in seconds.
	void update(float dt);
	/// @brief Resets the animation to the FIRST frame.
	void reset();
	/// @brief Submits the sprite for drawing.
	void draw();

private:
	Renderer *renderer = nullptr;

	std::unique_ptr<GPUMesh> mesh = nullptr;
	std::unique_ptr<Material> material = nullptr;

	Animation animation;
	size_t current_frame = 0;
	float frame_speed_sec = 0.1f;
	float frame_time_sec = 0.0f;

	Vec2 position;
	Vec2 scale = { 1.0f, 1.0f };
	Vec2 size = { 1.0f, 1.0f };
	float rotation = 0.0f;
	float layer = 0.0f;

	void applyFrame(const SliceUV &frame);
};

}  // namespace lili
