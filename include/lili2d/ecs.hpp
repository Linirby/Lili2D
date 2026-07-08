#pragma once

/// @file ecs.hpp
/// @brief Aggregates the Entity Component System (ECS) of the Lili2D engine.

#include "lili2d/ecs/entity.hpp"  // IWYU pragma: keep
#include "lili2d/ecs/component_pool.hpp"  // IWYU pragma: keep
#include "lili2d/ecs/ecs_registry.hpp"  // IWYU pragma: keep

namespace lili {

typedef uint32_t Entity;
/// @brief Manages entities and components.
class ECSRegistry;

}  // namespace lili
