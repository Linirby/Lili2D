#pragma once

#include <cstdint>

namespace lili {

/// \brief Manages time and framerate calculations.
class Clock {
public:
	/// \brief Constructs the Clock.
	Clock();

	/// \brief Destroys the Clock.
	~Clock() = default;

	/**
	 * \brief Sets the fixed delta time for physics or fixed updates.
	 * \param value The fixed delta time in seconds.
	 */
	void set_fixed_dt(float value);

	/// \brief Updates the clock, calculating delta time since the last update.
	void update();

	/**
	 * \brief Steps the fixed accumulator.
	 * \return True if a fixed step should be executed, false otherwise.
	 */
	bool step();

	/**
	 * \brief Gets the fixed delta time.
	 * \return The fixed delta time in seconds.
	 */
	float get_fixed_dt() const;

	/**
	 * \brief Gets the delta time since the last frame.
	 * \return The delta time in seconds.
	 */
	float get_dt() const;

	/**
	 * \brief Gets the interpolation alpha for rendering between fixed steps.
	 * \return The alpha value [0.0, 1.0].
	 */
	float get_alpha() const;

	/**
	 * \brief Gets the current frames per second.
	 * \return The frames per second.
	 */
	int get_fps() const;

private:
	uint64_t last;
	uint64_t now;

	float fixed_dt;
	float dt;
	float accumulator;
	float second_counter;

	int fps;
	int temp_fps;
};

}  // namespace lili
