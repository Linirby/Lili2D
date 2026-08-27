#include "lili2d/render/scene/common/animation.hpp"

namespace lili {

/// @brief A lightweight player to track animation state over time.
class AnimationPlayer {
public:
    /// @brief Default constructor.
    AnimationPlayer() = default;
    /// @brief Constructs a player for a specific animation.
    /// @param animation Pointer to the animation to play.
    explicit AnimationPlayer(const Animation* animation);
    /// @brief Constructs a player from a registered animation ID.
    /// @param animation_id The registered animation numeric ID.
    explicit AnimationPlayer(uint16_t animation_id);
    /// @brief Constructs a player from a registered animation key.
    /// @param animation_key The registered animation string key.
    explicit AnimationPlayer(const std::string& animation_key);

    /// @brief Sets the animation to play.
    /// @param animation Pointer to the Animation instance.
    void
    setAnimation(const Animation* animation);
    /// @brief Sets the animation to play from a registry ID.
    /// @param animation_id The registered animation numeric ID.
    void
    setAnimation(uint16_t animation_id);
    /// @brief Sets the animation to play from a registry key.
    /// @param animation_key The registered animation string key.
    void
    setAnimation(const std::string& animation_key);

    /// @brief Sets the speed of the animation.
    /// @param speed_sec Time per frame in seconds.
    void
    setFrameSpeed(float speed_sec);

    /// @brief Advances the animation by dt seconds.
    /// @param dt Delta time in seconds.
    void
    update(float dt);
    /// @brief Resets the animation to the first frame.
    void
    reset();

    /// @brief Gets the current frame of the animation.
    /// @return Reference to the current SliceUV.
    const SliceUV&
    getCurrentFrame() const;

private:
    /// @brief Pointer to the played Animation object.
    const Animation* animation = nullptr;
    /// @brief Current active frame index.
    size_t current_frame = 0;
    /// @brief Frame duration speed in seconds per frame.
    float frame_speed_sec = 0.1f;
    /// @brief Accumulated time spent on the current frame.
    float frame_time_sec = 0.0f;
};

}  // namespace lili
