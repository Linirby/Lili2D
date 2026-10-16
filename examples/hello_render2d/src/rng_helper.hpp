#pragma once

#include "lili2d/geometry/vec2.hpp"
#include "lili2d/geometry/vec4.hpp"
#include <random>
namespace rng {

std::mt19937&
gen();

lili::Vec2
pos(lili::Vec2 screen_size);

lili::Vec2
size();

float
radius();

lili::Vec4
color();

bool
hollow();

float
thickness();

} // namespace rng
