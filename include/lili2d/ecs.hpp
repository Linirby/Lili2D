#pragma once

/// @file ecs.hpp
/// @brief Aggregates the Entity Component System (ECS) of the Lili2D engine.

#include "lili2d/ecs/command_buffer.hpp"  // IWYU pragma: keep
#include "lili2d/ecs/component_pool.hpp"  // IWYU pragma: keep
#include "lili2d/ecs/ecs_registry.hpp"  // IWYU pragma: keep
#include "lili2d/ecs/entity.hpp"  // IWYU pragma: keep
#include "lili2d/ecs/system.hpp"  // IWYU pragma: keep

namespace lili {


/// @brief CommandBuffer defers structural ECS modifications.
class CommandBuffer;

/// @brief Contiguous component pool implementation for a specific type T.
template<typename T>
class ComponentPool;

/// @brief Manages entities and components.
class ECSRegistry;

typedef uint32_t Entity;

/// @brief Templated base class for ECS systems.
class SystemBase;
/// @brief Polymorphic base class for ECS systems.
template <typename T>
class System;

}  // namespace lili
