#pragma once

#include <lili2d/ecs/ecs_registry.hpp>
#include <lili2d/render/scene/core2d/sprite_batch.hpp>

namespace systems {

void
updateMovement(
    lili::ECSRegistry& registry, float dt, float window_w, float window_h
);
void
renderEntities(lili::ECSRegistry& registry, lili::SpriteBatch& batch);

}  // namespace systems
