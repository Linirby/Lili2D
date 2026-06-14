#include "clock.hpp"

namespace lili {

Clock::Clock(float tick_per_second) : fixed_dt(1.0f / tick_per_second) {}

void Clock::setTps(float value) {
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

float Clock::getFixedDt() const { return fixed_dt; }
float Clock::getDt() const { return dt; }
float Clock::getAlpha() const { return accumulator / fixed_dt; }
int Clock::getFps() const { return fps; }
float Clock::getTime() const { return SDL_GetTicks() / 1000.0f; }

}  // namespace lili
