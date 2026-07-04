#pragma once
#include <lili2d/lili2d.hpp>

namespace entities {
	lili::Entity spawnBall(
		lili::ECSRegistry &registry,
		lili::Vec2 pos,
		lili::Vec2 vel,
		lili::SliceUV slice,
		lili::Vec4 color,
		float radius
	);
}
