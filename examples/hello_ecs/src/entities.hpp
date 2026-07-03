#pragma once
#include <lili2d/lili2d.hpp>

namespace entities {
	lili::Entity spawnBall(
		lili::ECSRegistry &registry,
		lili::Renderer *renderer,
		lili::Vec2 pos,
		lili::Vec2 vel,
		float radius,
		lili::Vec4 color
	);
}
