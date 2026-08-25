#pragma once

#include "timer.hpp"

namespace lili {

/// @brief Manages a collection of active timers with automatic lifecycle
/// cleanup.
class TimerManager {
public:
    TimerManager() = default;
    ~TimerManager() = default;

    /// @brief Creates a managed timer.
    /// @param duration Duration in seconds.
    /// @param on_complete Callback executed when timer finishes.
    /// @param repeating Whether the timer repeats.
    /// @return Raw pointer to the created timer.
    Timer*
    create(
        float duration, Timer::CompleteCallback on_complete = nullptr,
        bool repeating = false
    );

    /// @brief Updates all managed timers and removes finished non-repeating
    /// timers.
    /// @param dt Delta time in seconds.
    void
    update(float dt);

    /// @brief Clears all managed timers.
    void
    clear();

    /// @brief Gets the number of currently active timers.
    size_t
    count() const;

private:
    std::vector<std::unique_ptr<Timer>> timers;
};

}  // namespace lili
