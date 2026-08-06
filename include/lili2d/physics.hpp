#pragma once

/// @file physics.hpp
/// @brief Aggregates the physics components of the Lili2D engine, including
/// collision detection and raycasting.

#include "lili2d/physics/aabb_collider.hpp"
#include "lili2d/physics/circle_collider.hpp"
#include "lili2d/physics/collision.hpp"
#include "lili2d/physics/raycast.hpp"

namespace lili {

/// @brief An axis-aligned bounding box 2D for collision detection.
struct AABB2;
/// @brief An axis-aligned bounding box 3D for collision detection.
struct AABB3;
/// @brief A 2D circle collider for collision detection.
struct CircleCollider;
/// @brief Result data from a raycast operation.
struct RaycastResult;

}  // namespace lili
