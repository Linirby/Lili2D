#pragma once

#include "lili2d/geometry/vec2.hpp"
#include "lili2d/geometry/vec4.hpp"
#include <random>
namespace rng {

std::mt19937&
gen();

lili::Vec2
pos();

lili::Vec2
size();

lili::Vec4
color();

} // namespace rng
