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
	int hit_x, hit_y, hit_z;
	///< The integer coordinates of the hit voxel/block.
	int adj_x, adj_y, adj_z;
	///< The integer coordinates of the adjacent voxel/block (surface normal).
};

}  // namespace lili
