#include "core/clock.hpp"

#include <SDL3/SDL_timer.h>

namespace lili {

Clock::Clock() {
	last = SDL_GetTicks();
	fixed_dt = 1.0f / 30.0f;
}

Clock::Clock(float tick_per_second) {
	last = SDL_GetTicks();
	fixed_dt = 1.0f / tick_per_second;
}

void Clock::set_tps(float value) {
	fixed_dt = 1.0f / value;
}

void Clock::update() {
	now = SDL_GetTicks();
	dt = (now - last) / 1000.0f;
	last = now;
	if (dt > 0.25f) {
		dt = 0.25f;
	}
	accumulator += dt;
	if (second_counter <= 1.0f) {
		second_counter += dt;
		temp_fps++;
	} else {
		fps = temp_fps;
		second_counter = 0.0f;
		temp_fps = 0;
	}
}

bool Clock::step() {
	if (accumulator >= fixed_dt) {
		accumulator -= fixed_dt;
		return true;
	}
	return false;
}

float Clock::get_fixed_dt() const { return fixed_dt; }
float Clock::get_dt() const { return dt; }
float Clock::get_alpha() const { return accumulator / fixed_dt; }
int Clock::get_fps() const { return fps; }

}  // namespace lili
