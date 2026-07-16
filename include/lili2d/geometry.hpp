#pragma once

/// @file geometry.hpp
/// @brief Aggregates the geometry components of the Lili2D engine, including
/// vectors, matrices, and math utilities.

#include "lili2d/geometry/mat3x3.hpp"  // IWYU pragma: keep
#include "lili2d/geometry/mat4x4.hpp"  // IWYU pragma: keep
#include "lili2d/geometry/point2.hpp"  // IWYU pragma: keep
#include "lili2d/geometry/point3.hpp"  // IWYU pragma: keep
#include "lili2d/geometry/utils.hpp"  // IWYU pragma: keep
#include "lili2d/geometry/vec2.hpp"  // IWYU pragma: keep
#include "lili2d/geometry/vec3.hpp"  // IWYU pragma: keep
#include "lili2d/geometry/vec4.hpp"  // IWYU pragma: keep

namespace lili {

/// @brief A 3x3 matrix for 2D transformations.
struct Mat3;
/// @brief A 4x4 matrix for 3D or advanced transformations.
struct Mat4;

/// @brief An integer-based 2D point.
struct Point2;
/// @brief An integer-based 3D point (chunk indexing).
struct Point3;

/// @brief Converts an angle from degrees to radians.
/// @param degrees The angle in degrees.
/// @return The angle in radians.
inline float degToRad(float degrees);

/// @brief A 2D vector.
struct Vec2;
/// @brief A 3D vector.
struct Vec3;
/// @brief A 4D vector.
struct Vec4;

}  // namespace lili
