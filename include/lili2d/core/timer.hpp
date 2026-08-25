#pragma once

#include <functional>
#include <memory>
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
    );

    /// @brief Advances the timer by delta time in seconds.
    /// @param dt Frame delta time in seconds.
    void
    update(float dt);

    /// @brief Starts or restarts the timer from zero.
    void
    start();

    /// @brief Pauses the timer without resetting elapsed time.
    void
    pause();

    /// @brief Resumes a paused timer.
    void
    resume();

    /// @brief Resets elapsed time to zero and stops the timer.
    void
    reset();

    /// @brief Restarts the timer with a new duration.
    /// @param new_duration The new duration in seconds.
    void
    restart(float new_duration);

    /// @brief Checks if the timer has reached its duration.
    bool
    isFinished() const;

    /// @brief Checks if the timer is actively running.
    bool
    isRunning() const;

    /// @brief Checks if the timer is paused.
    bool
    isPaused() const;

    /// @brief Gets normalized progress in the range [0.0, 1.0].
    float
    getProgress() const;

    /// @brief Gets elapsed time in seconds.
    float
    getElapsed() const;

    /// @brief Gets remaining time in seconds until completion.
    float
    getRemaining() const;

    /// @brief Gets the configured duration in seconds.
    float
    getDuration() const;

    /// @brief Sets the duration in seconds.
    void
    setDuration(float new_duration);

    /// @brief Sets whether the timer automatically restarts when finished.
    void
    setRepeating(bool is_repeating);

    /// @brief Registers a callback executed when the timer completes.
    Timer&
    onComplete(CompleteCallback callback);

    /// @brief Registers a callback executed every update tick with normalized
    /// progress [0.0, 1.0].
    Timer&
    onTick(TickCallback callback);

private:
    float duration = 0.0f;
    float elapsed = 0.0f;
    bool running = false;
    bool paused = false;
    bool repeating = false;

    CompleteCallback complete_callback = nullptr;
    TickCallback tick_callback = nullptr;
};

}  // namespace lili
