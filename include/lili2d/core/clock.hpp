#pragma once

#include <SDL3/SDL_timer.h>

#include <cstdint>

namespace lili {

/// @brief Manages time and framerate calculations.
class Clock {
public:
    /// @brief Constructs the Clock.
    Clock() noexcept = default;

    /// @brief Constructs the Clock with a target ticks per second rate.
    /// @param tick_per_second Target ticks per second (TPS).
    /// @param max_fps_cap Target maximum frames per second (0 = uncapped).
    explicit Clock(float tick_per_second, uint32_t max_fps_cap = 0) noexcept
        : max_fps(max_fps_cap), tps(tick_per_second), fixed_dt(1.0f / tick_per_second) {}

    /// @brief Sets the fixed delta time for physics or fixed updates.
    /// @param value The fixed delta time in seconds.
    inline void
    setTps(float value) noexcept {
        fixed_dt = 1.0f / value;
        tps = value;
    }

    /// @brief Sets the maximum frames per second cap.
    /// @param value Target maximum FPS (0 = uncapped).
    inline void
    setMaxFps(uint32_t value) noexcept {
        max_fps = value;
    }

    /// @brief Gets the maximum frames per second cap.
    /// @return Target maximum FPS (0 = uncapped).
    [[nodiscard]] constexpr uint32_t
    getMaxFps() const noexcept {
        return max_fps;
    }

    /// @brief Delays execution if needed to cap the frame rate to max_fps.
    void
    limitFps() noexcept;

    /// @brief Resets the frame timer (useful right before entering the game loop).
    void
    reset() noexcept;

    /// @brief Updates the clock, calculating delta time since the last update.
    void
    update() noexcept;

    /// @brief Steps the fixed accumulator.
    /// @return True if a fixed step should be executed, false otherwise.
    [[nodiscard]] inline bool
    step() noexcept {
        if (accumulator >= fixed_dt) {
            accumulator -= fixed_dt;
            return true;
        }
        return false;
    }

    /// @brief Gets the fixed delta time.
    /// @return The fixed delta time in seconds.
    [[nodiscard]] constexpr float
    getFixedDt() const noexcept {
        return fixed_dt;
    }

    /// @brief Gets the delta time since the last frame.
    /// @return The delta time in seconds.
    [[nodiscard]] constexpr float
    getDt() const noexcept {
        return dt;
    }

    /// @brief Gets the interpolation alpha for rendering between fixed steps.
    /// @return The alpha value [0.0, 1.0].
    [[nodiscard]] inline float
    getAlpha() const noexcept {
        return accumulator / fixed_dt;
    }

    /// @brief Gets the current frames per second.
    /// @return The frames per second.
    [[nodiscard]] constexpr int
    getFps() const noexcept {
        return fps;
    }

    /// @brief Gets the current time in seconds.
    /// @return The time value (float).
    [[nodiscard]] inline float
    getTime() const noexcept {
        return static_cast<float>(SDL_GetTicksNS()) / 1'000'000'000.0f;
    }

    /// @brief Gets the current tps.
    /// @return The tps (float).
    [[nodiscard]] constexpr float
    getTps() const noexcept {
        return tps;
    }

private:
    uint64_t last = SDL_GetTicksNS();
    uint64_t now = 0;

    uint32_t max_fps = 0;
    float tps = 20.0f;
    float fixed_dt = 1.0f / tps;
    float dt = 0.0f;
    float accumulator = 0.0f;
    float second_counter = 0.0f;

    int fps = 0;
    int temp_fps = 0;
};

}  // namespace lili
