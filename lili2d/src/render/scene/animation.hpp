#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

#include "render/core/texture.hpp"

namespace lili {

/// @brief Represents a single frame within an atlas texture.
struct AnimationFrame {
	Texture *texture = nullptr;
	float u_min = 0.0f, v_min = 0.0f;
	float u_max = 1.0f, v_max = 1.0f;
	float width = 0.0f, height = 0.0f;
};

/// @brief Represents an animation as a sequence of frames.
///
/// An Animation is a simple container for a list of AnimationFrame objects.
/// Use AtlasMap to load and slice a spritesheet, then extract Animation
/// objects from it.
class Animation {
public:
	/// @brief Default constructor (empty animation).
	Animation() = default;
	/// @brief Constructs an animation from a pre-built list of frames.
	/// @param frames The sequence of frames.
	Animation(const std::vector<AnimationFrame> &frames);

	/// @brief Gets the number of frames.
	/// @return The frame count.
	size_t frame_count() const;

	/// @brief Gets a frame by index.
	/// @param index The frame index.
	/// @return Reference to the AnimationFrame.
	const AnimationFrame& get_frame(size_t index) const;

private:
	std::vector<AnimationFrame> frames;
};

/// @brief Registry for managing animations globally.
class AnimationRegistry {
public:
	AnimationRegistry(const AnimationRegistry&) = delete;
	AnimationRegistry& operator=(const AnimationRegistry&) = delete;

	/// @brief Gets the singleton instance.
	/// @return Reference to the AnimationRegistry.
	static AnimationRegistry &get();

	/// @brief Registers a new animation.
	/// @param key The unique string key.
	/// @param animation The animation to register.
	/// @return The assigned animation ID.
	uint16_t register_animation(
		const std::string &key, const Animation &animation
	);

	/// @brief Checks if an animation exists.
	/// @param key The animation key.
	/// @return True if the animation exists.
	bool has_animation(const std::string &key) const;
	/// @brief Gets an animation ID by key.
	/// @param key The animation key.
	/// @return The animation ID.
	uint16_t get_animation_id(const std::string &key) const;
	/// @brief Gets an animation by key.
	/// @param key The animation key.
	/// @return Reference to the animation.
	const Animation &get_animation(const std::string &key) const;
	/// @brief Gets an animation by 16-bit ID.
	/// @param id The animation ID.
	/// @return Reference to the animation.
	const Animation &get_animation(uint16_t id) const;
	/// @brief Gets an animation by 8-bit ID.
	/// @param id The animation ID.
	/// @return Reference to the animation.
	const Animation &get_animation(uint8_t id) const;

	/// @brief Gets the total number of registered animations.
	/// @return The number of animations.
	size_t animation_count() const;
	/// @brief Gets the raw animation data array.
	/// @return Pointer to the animation data.
	const Animation *animation_data() const;

private:
	std::unordered_map<std::string, uint16_t> key_to_id;
	std::vector<Animation> id_to_animation;

	/// @brief AnimationRegistry method.
	AnimationRegistry();
};

}  // namespace lili
