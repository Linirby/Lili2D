#pragma once

#include <SDL3/SDL_events.h>

using Scancode = SDL_Scancode;

namespace lili {

/// \brief Manages keyboard state and input tracking.
class Keyboard {
public:
	/// \brief Constructs the Keyboard.
	Keyboard();

	/// \brief Destroys the Keyboard.
	~Keyboard() = default;

	/// \brief Updates the keyboard state for the current frame.
	void update();

	/**
	 * \brief Checks if a key is currently held down.
	 * \param key The scancode of the key to check.
	 * \return True if the key is held, false otherwise.
	 */
	bool held(Scancode key) const;

	/**
	 * \brief Checks if a key was pressed down in the current frame.
	 * \param key The scancode of the key to check.
	 * \return True if the key was just pressed, false otherwise.
	 */
	bool pressed(Scancode key) const;

	/**
	 * \brief Checks if a key was released in the current frame.
	 * \param key The scancode of the key to check.
	 * \return True if the key was just released, false otherwise.
	 */
	bool released(Scancode key) const;

private:
	const bool *current;	
	bool previous[SDL_SCANCODE_COUNT];
	int key_count;
};

}  // namespace lili
