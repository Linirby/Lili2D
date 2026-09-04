#pragma once

#include <lili2d/ecs/ecs_registry.hpp>
#include <lili2d/ecs/entity.hpp>
#include <lili2d/geometry/vec2.hpp>
#include <lili2d/geometry/vec4.hpp>
#include <lili2d/render/scene/common/atlas_map.hpp>

namespace entities {

lili::Entity
spawnBall(
    lili::ECSRegistry& registry, lili::Vec2 pos, lili::Vec2 vel,
    const lili::SliceUV& slice, lili::Vec4 color, float radius
);

}  // namespace entities
