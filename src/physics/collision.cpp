#include "lili2d/physics/collision.hpp"

#include <map>
#include <memory>

namespace lili {

AABB2::AABB2(const Vec2 &pos, const Vec2 &size) : min(pos), max(pos + size) {}

AABB2::AABB2(const Rect &rect)
	: min(rect.getPosition()), max(rect.getPosition() + rect.getSize()) {}

AABB2::AABB2(const Circle &circle)
	: min(circle.getTopLeft()),
	max(circle.getCenter() + Vec2(circle.getRadius(), circle.getRadius()))
{}

AABB2::AABB2(const Line &line) {
	Vec2 start = line.getStart();
	Vec2 end = line.getEnd();
	if (start.x < end.x && start.y < end.y) {
		min = start;
		max = end;
	} else if (start.x < end.x && start.y > end.y) {
		min = Vec2(start.x, end.y);
		max = Vec2(end.x, start.y);
	} else if (start.x > end.x && start.y > end.y) {
		min = end;
		max = start;
	} else {
		min = Vec2(end.x, start.y);
		max = Vec2(start.x, end.y);
	}
}

bool AABB2::intersect(const AABB2 &other) const {
	return (
		(min.x <= other.max.x && max.x >= other.min.x) &&
		(min.y <= other.max.y && max.y >= other.min.y)
	);
}

bool AABB2::intersect(const Rect &rect) const {
	return intersect(AABB2(rect));
}

bool AABB2::contains(const AABB2 &other) const {
	return (
		(min.x <= other.min.x && max.x >= other.max.x) &&
		(min.y <= other.min.y && max.y >= other.max.y)
	);
}

bool AABB2::contains(const Rect &rect) const {
	return contains(AABB2(rect));
}

bool AABB2::contains(const Circle &circle) const {
	return contains(AABB2(circle));
}

bool AABB2::contains(const Line &line) const {
	return contains(AABB2(line));
}

void AABB2::debugDraw(Renderer *renderer, const Vec4 &color) const {
	renderer->drawDebugRect(min.x, min.y, max.x - min.x, max.y - min.y, color);
}

AABB3::AABB3(const Vec3 &pos, const Vec3 &size) : min(pos), max(pos + size) {}

bool AABB3::intersect(const AABB3 &other) const {
	return (
		(min.x <= other.max.x && max.x >= other.min.x) &&
		(min.y <= other.max.y && max.y >= other.min.y) &&
		(min.z <= other.max.z && max.z >= other.min.z)
	);
}

bool AABB3::contains(const AABB3 &other) const {
	return (
		(min.x <= other.min.x && max.x >= other.max.x) &&
		(min.y <= other.min.y && max.y >= other.max.y) &&
		(min.z <= other.min.z && max.z >= other.max.z)
	);
}

}  // namespace lili
