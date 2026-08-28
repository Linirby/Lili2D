#pragma once

#include <cmath>
#include <functional>
#include <memory>
#include <utility>
#include <vector>

namespace lili {

/// @brief Represents a single countdown or interval timer.
class Timer {
public:
    using CompleteCallback = std::function<void()>;
    using TickCallback = std::function<void(float progress)>;

    /// @brief Constructs a Timer with a duration in seconds.
    /// @param duration Total timer duration in seconds.
    /// @param repeating Whether the timer restarts automatically upon
    /// completion.
    /// @param auto_start Whether the timer starts running immediately.
    explicit Timer(
        float duration = 0.0f, bool repeating = false, bool auto_start = true
    ) noexcept
        : duration(duration), repeating(repeating), running(auto_start) {}

    /// @brief Advances the timer by delta time in seconds.
    /// @param dt Frame delta time in seconds.
    inline void
    update(float dt) {
        if (!running || paused) return;
        elapsed += dt;
        if (tick_callback) tick_callback(getProgress());
        if (elapsed >= duration) {
            if (complete_callback) complete_callback();
            if (repeating)
                elapsed =
                    (duration > 0.0f) ? std::fmod(elapsed, duration) : 0.0f;
            else
                running = false;
        }
    }

    /// @brief Starts or restarts the timer from zero.
    inline void
    start() noexcept {
        elapsed = 0.0f;
        running = true;
        paused = false;
    }

    /// @brief Pauses the timer without resetting elapsed time.
    inline void
    pause() noexcept {
        paused = true;
    }

    /// @brief Resumes a paused timer.
    inline void
    resume() noexcept {
        paused = false;
    }

    /// @brief Resets elapsed time to zero and stops the timer.
    inline void
    reset() noexcept {
        elapsed = 0.0f;
        running = false;
        paused = false;
    }

    /// @brief Restarts the timer with a new duration.
    /// @param new_duration The new duration in seconds.
    inline void
    restart(float new_duration) noexcept {
        duration = new_duration;
        start();
    }

    /// @brief Checks if the timer has reached its duration.
    [[nodiscard]] inline bool
    isFinished() const noexcept {
        return !repeating && elapsed >= duration;
    }

    /// @brief Checks if the timer is actively running.
    [[nodiscard]] inline bool
    isRunning() const noexcept {
        return running && !paused;
    }

    /// @brief Checks if the timer is paused.
    [[nodiscard]] inline bool
    isPaused() const noexcept {
        return paused;
    }

    /// @brief Gets normalized progress in the range [0.0, 1.0].
    [[nodiscard]] inline float
    getProgress() const noexcept {
        if (duration <= 0.0f) return 1.0f;
        float p = elapsed / duration;
        return p > 1.0f ? 1.0f : p;
    }

    /// @brief Gets elapsed time in seconds.
    [[nodiscard]] inline float
    getElapsed() const noexcept {
        return elapsed;
    }

    /// @brief Gets remaining time in seconds until completion.
    [[nodiscard]] inline float
    getRemaining() const noexcept {
        return duration > elapsed ? duration - elapsed : 0.0f;
    }

    /// @brief Gets the configured duration in seconds.
    [[nodiscard]] inline float
    getDuration() const noexcept {
        return duration;
    }

    /// @brief Sets the duration in seconds.
    inline void
    setDuration(float new_duration) noexcept {
        duration = new_duration;
    }

    /// @brief Sets whether the timer automatically restarts when finished.
    inline void
    setRepeating(bool is_repeating) noexcept {
        repeating = is_repeating;
    }

    /// @brief Registers a callback executed when the timer completes.
    inline Timer&
    onComplete(CompleteCallback callback) {
        complete_callback = std::move(callback);
        return *this;
    }

    /// @brief Registers a callback executed every update tick with normalized
    /// progress [0.0, 1.0].
    inline Timer&
    onTick(TickCallback callback) {
        tick_callback = std::move(callback);
        return *this;
    }

private:
    float duration = 0.0f;
    float elapsed = 0.0f;
    bool repeating = false;
    bool running = false;
    bool paused = false;

    CompleteCallback complete_callback = nullptr;
    TickCallback tick_callback = nullptr;
};

}  // namespace lili
