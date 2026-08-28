#pragma once

#include <memory>
#include <utility>
#include <vector>

#include "lili2d/core/timer.hpp"

namespace lili {

/// @brief Manages a collection of active timers with automatic lifecycle
/// cleanup.
class TimerManager {
public:
    TimerManager() = default;
    ~TimerManager() = default;

    TimerManager(TimerManager&&) noexcept = default;
    TimerManager&
    operator=(TimerManager&&) noexcept = default;

    TimerManager(const TimerManager&) = delete;
    TimerManager&
    operator=(const TimerManager&) = delete;

    /// @brief Creates a managed timer.
    /// @param duration Duration in seconds.
    /// @param on_complete Callback executed when timer finishes.
    /// @param repeating Whether the timer repeats.
    /// @return Raw pointer to the created timer.
    inline Timer*
    create(
        float duration, Timer::CompleteCallback on_complete = nullptr,
        bool repeating = false
    ) {
        auto timer = std::make_unique<Timer>(duration, repeating, true);
        if (on_complete) timer->onComplete(std::move(on_complete));
        Timer* ptr = timer.get();
        timers.push_back(std::move(timer));
        return ptr;
    }

    /// @brief Updates all managed timers and removes finished non-repeating
    /// timers.
    /// @param dt Delta time in seconds.
    inline void
    update(float dt) {
        for (auto it = timers.begin(); it != timers.end();) {
            (*it)->update(dt);
            if ((*it)->isFinished())
                it = timers.erase(it);
            else
                ++it;
        }
    }

    /// @brief Clears all managed timers.
    inline void
    clear() noexcept {
        timers.clear();
    }

    /// @brief Gets the number of currently active timers.
    [[nodiscard]] inline size_t
    count() const noexcept {
        return timers.size();
    }

    /// @brief Checks if there are no active timers.
    [[nodiscard]] inline bool
    empty() const noexcept {
        return timers.empty();
    }

private:
    std::vector<std::unique_ptr<Timer>> timers;
};

}  // namespace lili
