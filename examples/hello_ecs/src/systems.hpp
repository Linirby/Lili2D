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
void
renderEntities(lili::ECSRegistry& registry, lili::DynamicBatch& batch);

} // namespace systems
