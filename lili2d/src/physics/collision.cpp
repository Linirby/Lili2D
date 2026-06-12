#include "physics/collision.hpp"

#include <map>
#include <memory>

namespace lili {

AABB::AABB(const Vec2 &pos, const Vec2 &size) : min(pos), max(pos + size) {}

AABB::AABB(const Rect &rect)
	: min(rect.getPosition()), max(rect.getPosition() + rect.getSize()) {}

AABB::AABB(const Circle &circle)
	: min(circle.getTopLeft()),
	max(circle.getCenter() + Vec2(circle.getRadius(), circle.getRadius()))
{}

AABB::AABB(const Line &line) {
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

bool AABB::intersect(const AABB &other) const {
	return (
		(min.x <= other.max.x && max.x >= other.min.x) &&
		(min.y <= other.max.y && max.y >= other.min.y)
	);
}

bool AABB::intersect(const Rect &rect) const {
	return intersect(AABB(rect));
}

bool AABB::contains(const AABB &other) const {
	return (
		(min.x <= other.min.x && max.x >= other.max.x) &&
		(min.y <= other.min.y && max.y >= other.max.y)
	);
}

bool AABB::contains(const Rect &rect) const {
	return contains(AABB(rect));
}

bool AABB::contains(const Circle &circle) const {
	return contains(AABB(circle));
}

bool AABB::contains(const Line &line) const {
	return contains(AABB(line));
}

void AABB::debugDraw(Renderer *renderer, const Vec4 &color) const {
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

}  // namespace lili
