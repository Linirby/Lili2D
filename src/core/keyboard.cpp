#include "lili2d/core/keyboard.hpp"

#include <algorithm>
#include <cstring>

namespace lili {

void
Keyboard::update() noexcept {
    std::memcpy(previous, current, SDL_SCANCODE_COUNT * sizeof(bool));
    int sdl_key_count = 0;
    const bool* sdl_state = SDL_GetKeyboardState(&sdl_key_count);
    if (sdl_state) {
        key_count = std::min(sdl_key_count, (int)SDL_SCANCODE_COUNT);
        std::memcpy(current, sdl_state, key_count * sizeof(bool));
        if (key_count < SDL_SCANCODE_COUNT)
            std::memset(
                current + key_count, 0,
                (SDL_SCANCODE_COUNT - key_count) * sizeof(bool)
            );
    }
}

}  // namespace lili
