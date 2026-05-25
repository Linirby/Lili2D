#pragma once

#include "geometry/vec3.hpp"

namespace lili {

struct AABB {
	Vec3 min;
	Vec3 max;

	bool intersect(const AABB &other) const;
};

struct RaycastResult {
	bool hit = false;
	int hit_x, hit_y, hit_z;
	int adj_x, adj_y, adj_z;
};

}  // namespace lili
