#pragma once

#include "geometry/vec3.hpp"

namespace lili {

/// @brief Represents an Axis-Aligned Bounding Box.
struct AABB {
	Vec3 min;  ///< The minimum coordinates.
	Vec3 max;  ///< The maximum coordinates.

	/// @brief Checks if this AABB intersects with another.
	/// @param other The other AABB to test against.
	/// @return True if the AABBs intersect, false otherwise.
	bool intersect(const AABB &other) const;
};

/// @brief Contains the result of a raycast operation.
struct RaycastResult {
	bool hit = false;  ///< True if the raycast hit something.
	int hit_x = 0; ///< Hit X coordinate.
	int hit_y = 0; ///< Hit Y coordinate.
	int hit_z = 0; ///< Hit Z coordinate.
	int adj_x = 0; ///< Adjacent X coordinate (surface normal).
	int adj_y = 0; ///< Adjacent Y coordinate (surface normal).
	int adj_z = 0; ///< Adjacent Z coordinate (surface normal).
};

}  // namespace lili
