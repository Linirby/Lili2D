#pragma once

/// @file physics.hpp
/// @brief Aggregates the physics components of the Lili2D engine, including
/// collision detection and raycasting.

#include "../physics/collision.hpp"  // IWYU pragma: keep

namespace lili {

/// @brief An axis-aligned bounding box 2D for collision detection.
struct AABB2;
/// @brief An axis-aligned bounding box 3D for collision detection.
struct AABB3;
/// @brief Result data from a raycast operation.
struct RaycastResult;

}  // namespace lili
