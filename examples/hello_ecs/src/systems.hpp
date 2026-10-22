#pragma once

#include <lili2d/ecs/ecs_registry.hpp>
#include <lili2d/render/2d/dynamic_batch.hpp>

namespace systems {

void
updateMovement(
    lili::ECSRegistry& registry,
    float dt,
    float window_w,
    float window_h
);

} // namespace systems
