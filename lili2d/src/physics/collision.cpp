#include "collision.hpp"

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
	static std::map<uint32_t, std::unique_ptr<Rect>> debug_rects;
	uint32_t r = (uint32_t)(color.x * 255.0f);
	uint32_t g = (uint32_t)(color.y * 255.0f);
	uint32_t b = (uint32_t)(color.z * 255.0f);
	uint32_t a = (uint32_t)(color.w * 255.0f);
	uint32_t key = (r << 24) | (g << 16) | (b << 8) | a;

	if (debug_rects.find(key) == debug_rects.end()) {
		debug_rects[key] = std::make_unique<Rect>(
			renderer, RectShape(0, 0, 0, 0), color
		);
		debug_rects[key]->setHollow(true);
	}

	debug_rects[key]->setShape(
		RectShape(min.x, min.y, max.x - min.x, max.y - min.y)
	);
	debug_rects[key]->draw();
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
