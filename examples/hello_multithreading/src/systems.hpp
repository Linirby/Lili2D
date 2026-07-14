#pragma once

#include "lili2d/ecs.hpp"
#include "lili2d/core/thread_pool.hpp"
#include "components.hpp"

namespace systems {

class MovementSystem : public lili::System<PositionComponent> {
public:
	MovementSystem(float window_w, float window_h);

	void updateEntity(lili::ECSRegistry& registry, lili::Entity entity, PositionComponent& pos, float dt) override;

private:
	float window_w;
	float window_h;
};

} // namespace systems
