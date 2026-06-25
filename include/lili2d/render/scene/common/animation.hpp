#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

#include "lili2d/render/scene/common/atlas_map.hpp"

namespace lili {

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
	Animation(const std::vector<SliceUV> &frames);

	/// @brief Gets the number of frames.
	/// @return The frame count.
	size_t frameCount() const;

	/// @brief Gets a frame by index.
	/// @param index The frame index.
	/// @return Reference to the SliceUV.
	const SliceUV& getFrame(size_t index) const;

private:
	std::vector<SliceUV> frames;
};

/// @brief A lightweight player to track animation state over time.
class AnimationPlayer {
public:
	/// @brief Default constructor.
	AnimationPlayer() = default;
	/// @brief Constructs a player for a specific animation.
	/// @param animation Pointer to the animation to play.
	AnimationPlayer(const Animation *animation);
	/// @brief Constructs a player from a registered animation ID.
	AnimationPlayer(uint16_t animation_id);
	/// @brief Constructs a player from a registered animation key.
	AnimationPlayer(const std::string &animation_key);

	/// @brief Sets the animation to play.
	void setAnimation(const Animation *animation);
	/// @brief Sets the animation to play from a registry ID.
	void setAnimation(uint16_t animation_id);
	/// @brief Sets the animation to play from a registry key.
	void setAnimation(const std::string &animation_key);

	/// @brief Sets the speed of the animation.
	/// @param speed_sec Time per frame in seconds.
	void setFrameSpeed(float speed_sec);

	/// @brief Advances the animation by dt seconds.
	/// @param dt Delta time in seconds.
	void update(float dt);
	/// @brief Resets the animation to the first frame.
	void reset();

	/// @brief Gets the current frame of the animation.
	/// @return Reference to the current SliceUV.
	const SliceUV& getCurrentFrame() const;

private:
	const Animation *animation = nullptr;
	size_t current_frame = 0;
	float frame_speed_sec = 0.1f;
	float frame_time_sec = 0.0f;
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

	/// @brief Private constructor to enforce singleton pattern.
	AnimationRegistry();
};

}  // namespace lili
