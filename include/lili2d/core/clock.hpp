#pragma once

#include <SDL3/SDL_timer.h>

#include <cstdint>

namespace lili {

/// @brief Manages time and framerate calculations.
class Clock {
public:
    /// @brief Constructs the Clock.
    Clock() = default;
    /// @brief Constructs the Clock with a target ticks per second rate.
    /// @param tick_per_second Target ticks per second (TPS).
    explicit Clock(float tick_per_second);

    /// @brief Sets the fixed delta time for physics or fixed updates.
    /// @param value The fixed delta time in seconds.
    void
    setTps(float value);

    /// @brief Updates the clock, calculating delta time since the last update.
    void
    update();
    /// @brief Steps the fixed accumulator.
    /// @return True if a fixed step should be executed, false otherwise.
    bool
    step();

    /// @brief Gets the fixed delta time.
    /// @return The fixed delta time in seconds.
    float
    getFixedDt() const;
    /// @brief Gets the delta time since the last frame.
    /// @return The delta time in seconds.
    float
    getDt() const;
    /// @brief Gets the interpolation alpha for rendering between fixed steps.
    /// @return The alpha value [0.0, 1.0].
    float
    getAlpha() const;
    /// @brief Gets the current frames per second.
    /// @return The frames per second.
    int
    getFps() const;
    /// @brief Gets the current time in seconds.
    /// @return The time value (float).
    float
    getTime() const;
    /// @brief Gets the current tps.
    /// @return The tps (float).
    float
    getTps() const;

private:
    /// @brief Timestamp of the previous update tick in milliseconds.
    uint64_t last = SDL_GetTicks();
    /// @brief Timestamp of the current update tick in milliseconds.
    uint64_t now = 0;

    /// @brief Target ticks per second.
    float tps = 20.0f;
    /// @brief Fixed delta time per tick in seconds.
    float fixed_dt = 1.0f / tps;
    /// @brief Frame delta time in seconds.
    float dt = 0.0f;
    /// @brief Accumulated time for fixed timestep updates.
    float accumulator = 0.0f;
    /// @brief Accumulated time for FPS calculation.
    float second_counter = 0.0f;

    /// @brief Calculated frames per second.
    int fps = 0;
    /// @brief Temporary frame counter during one second.
    int temp_fps = 0;
};

}  // namespace lili
