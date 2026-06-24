#pragma once

#include "lili2d/core/event.hpp"
#include "lili2d/geometry/vec2.hpp"

namespace lili {

/// @brief Manages mouse state and input tracking.
class Mouse {
public:
	/// @brief Updates the mouse state for the current frame.
	void update();
	/// @brief Checks if a mouse button is currently held down.
	/// @param button The mouse button to check.
	/// @return True if the button is held, false otherwise.
	bool held(MouseButton button) const;
	/// @brief Checks if a mouse button was pressed down in the current frame.
	/// @param button The mouse button to check.
	/// @return True if the button was just pressed, false otherwise.
	bool pressed(MouseButton button) const;
	/// @brief Checks if a mouse button was released in the current frame.
	/// @param button The mouse button to check.
	/// @return True if the button was just released, false otherwise.
	bool released(MouseButton button) const;

	/// @brief Gets the current X position of the mouse.
	/// @return The X position.
	float getX() const;
	/// @brief Gets the current Y position of the mouse.
	/// @return The Y position.
	float getY() const;
	/// @brief Gets the current X, Y position of the mouse.
	/// @return The 2D vector of X, Y position.
	Vec2 getPos() const;
	/// @brief Gets the change in X position since the last frame.
	/// @return The change in X.
	float getDx() const;
	/// @brief Gets the change in Y position since the last frame.
	/// @return The change in Y.
	float getDy() const;

private:
	uint32_t current = 0;
	uint32_t previous = 0;
	float current_x = 0.0f;
	float current_y = 0.0f;
	float current_dx = 0.0f;
	float current_dy = 0.0f;
};

}  // namespace lili
