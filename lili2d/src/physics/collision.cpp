#include "physics/collision.hpp"

namespace lili {

AABB::AABB(const Vec2 &pos, const Vec2 &size) {
	min = pos;
	max = pos + size;
}

bool AABB::intersect(const AABB &other) const {
	return (
		(min.x <= other.max.x && max.x >= other.min.x) &&
		(min.y <= other.max.y && max.y >= other.min.y)
	);
}

}  // namespace lili

