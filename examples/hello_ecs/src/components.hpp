#pragma once
#include <lili2d/lili2d.hpp>

struct PositionComponent {
	lili::Vec2 value;
};

struct VelocityComponent {
	lili::Vec2 value;
};

struct RenderComponent {
	lili::Circle circle;

	RenderComponent(
		lili::Renderer* renderer,
		lili::Vec2 center,
		float radius,
		lili::Vec4 color
	) : circle(renderer, lili::CircleShape(center, radius, 32), color) {}
};
