#include "lili2d/render/scene/common/animation.hpp"

namespace lili {

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

}  // namespace lili
