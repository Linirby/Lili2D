#pragma once

#include "SDL3/SDL_events.h"

using Keycode = SDL_Keycode;
using Scancode = SDL_Scancode;

namespace lili {

/// \brief Types of events that can occur.
enum class EventType {
	NONE,
	QUIT,
	WINDOW_RESIZED,
	KEYBOARD,
	MOUSE_BUTTON,
	MOUSE_MOTION,
	MOUSE_WHEEL
};

/// \brief Represents keyboard key action types.
enum class KeyAction {
	PRESSED,
	RELEASED
};

/// \brief Represents mouse buttons.
enum class MouseButton {
	UNKNOWN,
	LEFT,
	MIDDLE,
	RIGHT
};

/// \brief Represents mouse button action types.
enum class MouseAction {
	PRESSED,
	RELEASED
};

/// \brief Contains data for a keyboard event.
struct KeyboardEvent {
	Keycode key;       ///< The keycode of the key.
	Scancode scancode; ///< The scancode of the key.
	KeyAction action;  ///< The action performed on the key.
	bool repeat;       ///< True if this is a key repeat event.
};

/// \brief Contains data for a mouse button event.
struct MouseButtonEvent {
	MouseButton button; ///< The mouse button involved.
	MouseAction action; ///< The action performed on the mouse button.
	float x;            ///< The X coordinate of the mouse.
	float y;            ///< The Y coordinate of the mouse.
};

/// \brief Contains data for a mouse motion event.
struct MouseMotionEvent {
	float x;  ///< The X coordinate of the mouse.
	float y;  ///< The Y coordinate of the mouse.
	float dx; ///< The change in X since the last event.
	float dy; ///< The change in Y since the last event.
};

/// \brief Contains data for a mouse wheel event.
struct MouseWheelEvent {
	float dx; ///< The horizontal scroll amount.
	float dy; ///< The vertical scroll amount.
};

/// \brief Processes and provides access to system events.
class Event {
public:
	/// \brief Constructs the Event system.
	Event();

	/// \brief Destroys the Event system.
	~Event() = default;

	/**
	 * \brief Polls the next event from the queue.
	 * \return True if an event was processed, false if the queue is empty.
	 */
	bool poll();

	/**
	 * \brief Gets the type of the current event.
	 * \return The current event type.
	 */
	EventType type() const;

	/**
	 * \brief Checks if a specific key was just pressed based on an event.
	 * \param keyboard The keyboard event to check against.
	 * \return True if the key was just pressed, false otherwise.
	 */
	bool key_just_pressed(const KeyboardEvent &keyboard) const;

	/**
	 * \brief Checks if a specific mouse button was pressed based on an event.
	 * \param keyboard The mouse button event to check against.
	 * \return True if the button was pressed, false otherwise.
	 */
	bool mouse_button_pressed(const MouseButtonEvent &keyboard) const;

	/**
	 * \brief Gets the keyboard event data.
	 * \return The keyboard event data.
	 */
	const KeyboardEvent keyboard() const;

	/**
	 * \brief Gets the mouse button event data.
	 * \return The mouse button event data.
	 */
	const MouseButtonEvent mouse_button() const;

	/**
	 * \brief Gets the mouse motion event data.
	 * \return The mouse motion event data.
	 */
	const MouseMotionEvent mouse_motion() const;

	/**
	 * \brief Gets the mouse wheel event data.
	 * \return The mouse wheel event data.
	 */
	const MouseWheelEvent mouse_wheel() const;

private:
	SDL_Event sdl_event;
	EventType current_type;

	/// \brief resolve_type method.
	EventType resolve_type() const;
};

}  // namespace lili
