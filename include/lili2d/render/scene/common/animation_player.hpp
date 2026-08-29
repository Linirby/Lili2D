#pragma once

#include <cstdint>
#include <string_view>

#include "lili2d/render/scene/common/animation.hpp"

namespace lili {

/// @brief A lightweight player to track animation state over time.
class AnimationPlayer {
public:
    /// @brief Default constructor.
    AnimationPlayer() noexcept = default;
    /// @brief Constructs a player for a specific animation.
    /// @param animation Pointer to the animation to play.
    explicit AnimationPlayer(const Animation* animation) noexcept
        : animation(animation) {}
    /// @brief Constructs a player from a registered animation ID.
    /// @param animation_id The registered animation numeric ID.
    explicit AnimationPlayer(uint16_t animation_id);
    /// @brief Constructs a player from a registered animation key.
    /// @param animation_key The registered animation string key.
    explicit AnimationPlayer(std::string_view animation_key);

    /// @brief Move constructor.
    AnimationPlayer(AnimationPlayer&&) noexcept = default;
    /// @brief Move assignment operator.
    AnimationPlayer&
    operator=(AnimationPlayer&&) noexcept = default;

    /// @brief Copy constructor.
    AnimationPlayer(const AnimationPlayer&) = default;
    /// @brief Copy assignment operator.
    AnimationPlayer&
    operator=(const AnimationPlayer&) = default;

    /// @brief Sets the animation to play.
    /// @param animation Pointer to the Animation instance.
    inline void
    setAnimation(const Animation* animation) noexcept {
        this->animation = animation;
        reset();
    }
    /// @brief Sets the animation to play from a registry ID.
    /// @param animation_id The registered animation numeric ID.
    void
    setAnimation(uint16_t animation_id);
    /// @brief Sets the animation to play from a registry key.
    /// @param animation_key The registered animation string key.
    void
    setAnimation(std::string_view animation_key);

    /// @brief Sets the speed of the animation.
    /// @param speed_sec Time per frame in seconds.
    inline void
    setFrameSpeed(float speed_sec) noexcept {
        frame_speed_sec = speed_sec;
    }

    /// @brief Advances the animation by dt seconds.
    /// @param dt Delta time in seconds.
    void
    update(float dt) noexcept;
    /// @brief Resets the animation to the first frame.
    inline void
    reset() noexcept {
        current_frame = 0;
        frame_time_sec = 0.0f;
    }

    /// @brief Gets the current frame of the animation.
    /// @return Reference to the current SliceUV.
    [[nodiscard]] const SliceUV&
    getCurrentFrame() const noexcept;

    /// @brief Gets the played animation.
    /// @return Pointer to current Animation.
    [[nodiscard]] inline const Animation*
    getAnimation() const noexcept {
        return animation;
    }

    /// @brief Gets the active frame index.
    /// @return Frame index.
    [[nodiscard]] inline size_t
    getCurrentFrameIndex() const noexcept {
        return current_frame;
    }

    /// @brief Gets the frame duration speed in seconds.
    /// @return Time per frame in seconds.
    [[nodiscard]] inline float
    getFrameSpeed() const noexcept {
        return frame_speed_sec;
    }

private:
    const Animation* animation = nullptr;
    size_t current_frame = 0;
    float frame_speed_sec = 0.1f;
    float frame_time_sec = 0.0f;
};

}  // namespace lili
