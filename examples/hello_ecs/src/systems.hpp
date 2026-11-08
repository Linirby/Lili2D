#pragma once

#include <lili2d/ecs/ecs_registry.hpp>

namespace systems {

void
updateMovement(
    lili::ECSRegistry& registry,
    float dt,
    float window_w,
    float window_h
);

} // namespace systems
