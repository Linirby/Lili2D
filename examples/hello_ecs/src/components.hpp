#pragma once
#include <lili2d/lili2d.hpp>

struct PositionComponent {
	lili::Vec2 value;
};

struct VelocityComponent {
	lili::Vec2 value;
};

struct RenderComponent {
	lili::SliceUV slice;
	lili::Vec4 color;
	float radius;
};
