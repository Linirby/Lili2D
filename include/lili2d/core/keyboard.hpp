#pragma once

#include <SDL3/SDL_events.h>

using Scancode = SDL_Scancode;

namespace lili {

/// @brief Manages keyboard state and input tracking.
class Keyboard {
public:
    /// @brief Updates the keyboard state for the current frame.
    void
    update() noexcept;

    /// @brief Checks if a key is currently held down.
    /// @param key The scancode of the key to check.
    /// @return True if the key is held, false otherwise.
    [[nodiscard]] inline bool
    held(Scancode key) const noexcept {
        return key < SDL_SCANCODE_COUNT && current[key];
    }

    /// @brief Checks if a key was pressed down in the current frame.
    /// @param key The scancode of the key to check.
    /// @return True if the key was just pressed, false otherwise.
    [[nodiscard]] inline bool
    justPressed(Scancode key) const noexcept {
        return key < SDL_SCANCODE_COUNT && current[key] && !previous[key];
    }

    /// @brief Checks if a key was released in the current frame.
    /// @param key The scancode of the key to check.
    /// @return True if the key was just released, false otherwise.
    [[nodiscard]] inline bool
    justReleased(Scancode key) const noexcept {
        return key < SDL_SCANCODE_COUNT && !current[key] && previous[key];
    }

private:
    /// @brief Current frame key states array.
    bool current[SDL_SCANCODE_COUNT] = {};
    /// @brief Previous frame key states array.
    bool previous[SDL_SCANCODE_COUNT] = {};
    /// @brief Number of keys.
    int key_count = 0;
};

}  // namespace lili
