#pragma once

#include <cstdint>
#include <SDL3/SDL_timer.h>


namespace lili {

/// @brief Manages time and framerate calculations.
class Clock {
public:
	/// @brief Constructs the Clock.
	Clock() = default;
	/// @brief Clock method.
	Clock(float tick_per_second);

	/// @brief Sets the fixed delta time for physics or fixed updates.
	/// @param value The fixed delta time in seconds.
	void set_tps(float value);

	/// @brief Updates the clock, calculating delta time since the last update.
	void update();
	/// @brief Steps the fixed accumulator.
	/// @return True if a fixed step should be executed, false otherwise.
	bool step();

	/// @brief Gets the fixed delta time.
	/// @return The fixed delta time in seconds.
	float get_fixed_dt() const;
	/// @brief Gets the delta time since the last frame.
	/// @return The delta time in seconds.
	float get_dt() const;
	/// @brief Gets the interpolation alpha for rendering between fixed steps.
	/// @return The alpha value [0.0, 1.0].
	float get_alpha() const;
	/// @brief Gets the current frames per second.
	/// @return The frames per second.
	int get_fps() const;
	/// @brief Gets the current time in seconds.
	/// @return The time value (float)
	float get_time() const;


private:
	uint64_t last = SDL_GetTicks();
	uint64_t now = 0;

	float fixed_dt = 1.0f / 20.0f;
	float dt = 0.0f;
	float accumulator = 0.0f;
	float second_counter = 0.0f;

	int fps = 0;
	int temp_fps = 0;
};

}  // namespace lili
