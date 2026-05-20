#pragma once

#include <cstdint>

namespace lili {

class Clock {
public:
	Clock();
	~Clock() = default;

	void set_fixed_dt(float value);

	void update();
	bool step();
	float get_fixed_dt() const;
	float get_dt() const;
	float get_alpha() const;
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
