#include "lili2d/core/mouse.hpp"

#include <SDL3/SDL_events.h>

namespace lili {

void
Mouse::update() noexcept {
    previous = current;
    current_dx = 0.0f;
    current_dy = 0.0f;
    current = SDL_GetMouseState(&current_x, &current_y);
    SDL_GetRelativeMouseState(&current_dx, &current_dy);
}

}  // namespace lili
