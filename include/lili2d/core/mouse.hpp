#pragma once

#include <SDL3/SDL_mouse.h>

#include "lili2d/core/event.hpp"
#include "lili2d/geometry/vec2.hpp"

namespace lili {

/// @brief Manages mouse state and input tracking.
class Mouse {
public:
    /// @brief Updates the mouse state for the current frame.
    void
    update() noexcept;

    /// @brief Checks if a mouse button is currently held down.
    /// @param button The mouse button to check.
    /// @return True if the button is held, false otherwise.
    [[nodiscard]] inline bool
    held(MouseButton button) const noexcept {
        if (button == MouseButton::LEFT)
            return (current & SDL_BUTTON_LMASK) != 0;
        else if (button == MouseButton::MIDDLE)
            return (current & SDL_BUTTON_MMASK) != 0;
        else if (button == MouseButton::RIGHT)
            return (current & SDL_BUTTON_RMASK) != 0;
        return false;
    }

    /// @brief Checks if a mouse button was pressed down in the current frame.
    /// @param button The mouse button to check.
    /// @return True if the button was just pressed, false otherwise.
    [[nodiscard]] inline bool
    justPressed(MouseButton button) const noexcept {
        if (button == MouseButton::LEFT)
            return (current & SDL_BUTTON_LMASK) &&
                   !(previous & SDL_BUTTON_LMASK);
        else if (button == MouseButton::MIDDLE)
            return (current & SDL_BUTTON_MMASK) &&
                   !(previous & SDL_BUTTON_MMASK);
        else if (button == MouseButton::RIGHT)
            return (current & SDL_BUTTON_RMASK) &&
                   !(previous & SDL_BUTTON_RMASK);
        return false;
    }

    /// @brief Checks if a mouse button was released in the current frame.
    /// @param button The mouse button to check.
    /// @return True if the button was just released, false otherwise.
    [[nodiscard]] inline bool
    justReleased(MouseButton button) const noexcept {
        if (button == MouseButton::LEFT)
            return !(current & SDL_BUTTON_LMASK) &&
                   (previous & SDL_BUTTON_LMASK);
        else if (button == MouseButton::MIDDLE)
            return !(current & SDL_BUTTON_MMASK) &&
                   (previous & SDL_BUTTON_MMASK);
        else if (button == MouseButton::RIGHT)
            return !(current & SDL_BUTTON_RMASK) &&
                   (previous & SDL_BUTTON_RMASK);
        return false;
    }

    /// @brief Gets the current X position of the mouse.
    /// @return The X position.
    [[nodiscard]] inline float
    getX() const noexcept {
        return current_x;
    }

    /// @brief Gets the current Y position of the mouse.
    /// @return The Y position.
    [[nodiscard]] inline float
    getY() const noexcept {
        return current_y;
    }

    /// @brief Gets the current X, Y position of the mouse.
    /// @return The 2D vector of X, Y position.
    [[nodiscard]] inline Vec2
    getPos() const noexcept {
        return {current_x, current_y};
    }

    /// @brief Gets the change in X position since the last frame.
    /// @return The change in X.
    [[nodiscard]] inline float
    getDx() const noexcept {
        return current_dx;
    }

    /// @brief Gets the change in Y position since the last frame.
    /// @return The change in Y.
    [[nodiscard]] inline float
    getDy() const noexcept {
        return current_dy;
    }

private:
    /// @brief Current frame mouse button state mask.
    uint32_t current = 0;
    /// @brief Previous frame mouse button state mask.
    uint32_t previous = 0;
    /// @brief Current mouse X coordinate.
    float current_x = 0.0f;
    /// @brief Current mouse Y coordinate.
    float current_y = 0.0f;
    /// @brief Delta mouse movement along X axis.
    float current_dx = 0.0f;
    /// @brief Delta mouse movement along Y axis.
    float current_dy = 0.0f;
};

}  // namespace lili
