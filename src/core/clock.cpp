#include "lili2d/core/clock.hpp"

#include <SDL3/SDL_timer.h>

namespace lili {

Clock::Clock() noexcept : last(SDL_GetTicksNS()) {}

Clock::Clock(float tick_per_second, uint32_t max_fps_cap) noexcept
    : last(SDL_GetTicksNS()),
      max_fps(max_fps_cap),
      tps(tick_per_second),
      fixed_dt(1.0f / tick_per_second) {}

float
Clock::getTime() const noexcept {
    return static_cast<float>(SDL_GetTicksNS()) / 1'000'000'000.0f;
}

void
Clock::update() noexcept {
    now = SDL_GetTicksNS();
    dt = static_cast<float>(now - last) / 1'000'000'000.0f;
    last = now;
    if (dt > 0.25f) dt = 0.25f;
    accumulator += dt;
    if (second_counter <= 1.0f) {
        second_counter += dt;
        temp_fps++;
    } else {
        fps = temp_fps;
        second_counter = 0.0f;
        temp_fps = 0;
    }
}

void
Clock::limitFps() noexcept {
    if (max_fps == 0) return;

    const uint64_t target_ns = 1'000'000'000ULL / max_fps;
    const uint64_t current_ns = SDL_GetTicksNS();
    if (current_ns >= last) {
        const uint64_t elapsed_ns = current_ns - last;
        if (elapsed_ns < target_ns) {
            SDL_DelayPrecise(target_ns - elapsed_ns);
        }
    }
}

void
Clock::reset() noexcept {
    last = SDL_GetTicksNS();
    dt = 0.0f;
    accumulator = 0.0f;
    second_counter = 0.0f;
    temp_fps = 0;
}

}  // namespace lili
