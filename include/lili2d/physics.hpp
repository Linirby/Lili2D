#pragma once

/// @file physics.hpp
/// @brief Aggregates the physics components of the Lili2D engine, including
/// collision detection and raycasting.

#include "lili2d/physics/collision.hpp"  // IWYU pragma: keep

namespace lili {

/// @brief An axis-aligned bounding box for collision detection.
struct AABB2;
/// @brief Result data from a raycast operation.
struct RaycastResult;

}  // namespace lili
