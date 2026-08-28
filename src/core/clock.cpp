#include "lili2d/core/clock.hpp"

namespace lili {

void
Clock::update() noexcept {
    now = SDL_GetTicks();
    dt = (now - last) / 1000.0f;
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

}  // namespace lili
