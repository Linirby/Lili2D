#pragma once
#include <lili2d/lili2d.hpp>

namespace systems {

void updateMovement(
	lili::ECSRegistry &registry, float dt, float window_w, float window_h
);
void renderEntities(lili::ECSRegistry &registry, lili::SpriteBatch &batch);

}  // namespace systems
