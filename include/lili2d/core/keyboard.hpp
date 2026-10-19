#pragma once

#include "lili2d/core/keys.hpp"
#include <SDL3/SDL_scancode.h>

using Scancode = SDL_Scancode;

namespace lili {

/// @brief Manages keyboard state and input tracking.
class Keyboard
{
public:
    /// @brief Updates the keyboard state for the current frame.
    void
    update() noexcept;

    /// @brief Checks if a key is currently held down.
    /// @param key The scancode of the key to check.
    /// @return True if the key is held, false otherwise.
    [[nodiscard]] inline bool
    held(Key key) const noexcept
    {
        SDL_Scancode sdl_scancode = static_cast<SDL_Scancode>(key);
        return sdl_scancode < SDL_SCANCODE_COUNT && current[sdl_scancode];
    }

    /// @brief Checks if a key was pressed down in the current frame.
    /// @param key The scancode of the key to check.
    /// @return True if the key was just pressed, false otherwise.
    [[nodiscard]] inline bool
    justPressed(Key key) const noexcept
    {
        SDL_Scancode sdl_scancode = static_cast<SDL_Scancode>(key);
        return sdl_scancode < SDL_SCANCODE_COUNT && current[sdl_scancode] &&
               !previous[sdl_scancode];
    }

    /// @brief Checks if a key was released in the current frame.
    /// @param key The scancode of the key to check.
    /// @return True if the key was just released, false otherwise.
    [[nodiscard]] inline bool
    justReleased(Key key) const noexcept
    {
        SDL_Scancode sdl_scancode = static_cast<SDL_Scancode>(key);
        return sdl_scancode < SDL_SCANCODE_COUNT && !current[sdl_scancode] &&
               previous[sdl_scancode];
    }

private:
    bool current[SDL_SCANCODE_COUNT] = {};
    bool previous[SDL_SCANCODE_COUNT] = {};
    int key_count = 0;
};

} // namespace lili
