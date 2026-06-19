#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

#include "lili2d/render/core/texture.hpp"

namespace lili {

/// @brief Represents a single frame within an atlas texture.
struct AnimationFrame {
	Texture *texture = nullptr; ///< The texture.
	float u_min = 0.0f; ///< Minimum U coordinate.
	float v_min = 0.0f; ///< Minimum V coordinate.
	float u_max = 1.0f; ///< Maximum U coordinate.
	float v_max = 1.0f; ///< Maximum V coordinate.
	float width = 0.0f; ///< Frame width.
	float height = 0.0f; ///< Frame height.
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
	size_t frameCount() const;

	/// @brief Gets a frame by index.
	/// @param index The frame index.
	/// @return Reference to the AnimationFrame.
	const AnimationFrame& getFrame(size_t index) const;

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
	uint16_t registerAnimation(
		const std::string &key, const Animation &animation
	);

	/// @brief Checks if an animation exists.
	/// @param key The animation key.
	/// @return True if the animation exists.
	bool hasAnimation(const std::string &key) const;
	/// @brief Gets an animation ID by key.
	/// @param key The animation key.
	/// @return The animation ID.
	uint16_t getAnimationId(const std::string &key) const;
	/// @brief Gets an animation by key.
	/// @param key The animation key.
	/// @return Reference to the animation.
	const Animation &getAnimation(const std::string &key) const;
	/// @brief Gets an animation by 16-bit ID.
	/// @param id The animation ID.
	/// @return Reference to the animation.
	const Animation &getAnimation(uint16_t id) const;
	/// @brief Gets an animation by 8-bit ID.
	/// @param id The animation ID.
	/// @return Reference to the animation.
	const Animation &getAnimation(uint8_t id) const;

	/// @brief Gets the total number of registered animations.
	/// @return The number of animations.
	size_t animationCount() const;
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
