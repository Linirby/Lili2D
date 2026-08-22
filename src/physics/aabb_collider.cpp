#include "lili2d/physics/aabb_collider.hpp"

#include <algorithm>

#include "lili2d/physics/circle_collider.hpp"

namespace lili {

AABB2::AABB2(const Vec2& pos, const Vec2& size) : min(pos), max(pos + size) {}

AABB2::AABB2(const RectShape& rect)
    : min(Vec2(rect.x, rect.y)), max(Vec2(rect.x + rect.w, rect.y + rect.h)) {}

AABB2::AABB2(const CircleShape& circle)
    : min(circle.center - Vec2(circle.radius, circle.radius)),
      max(circle.center + Vec2(circle.radius, circle.radius)) {}

AABB2::AABB2(const LineShape& line) {
    min = Vec2(
        std::min(line.start.x, line.end.x), std::min(line.start.y, line.end.y)
    );
    max = Vec2(
        std::max(line.start.x, line.end.x), std::max(line.start.y, line.end.y)
    );
}

bool
AABB2::intersect(const AABB2& other) const {
    return (
        (min.x <= other.max.x && max.x >= other.min.x) &&
        (min.y <= other.max.y && max.y >= other.min.y)
    );
}

bool
AABB2::intersect(const RectShape& rect) const {
    return intersect(AABB2(rect));
}

bool
AABB2::intersect(const CircleCollider& circle) const {
    return circle.intersect(*this);
}

bool
AABB2::intersect(const CircleShape& circle) const {
    return CircleCollider(circle).intersect(*this);
}

bool
AABB2::contains(const AABB2& other) const {
    return (
        (min.x <= other.min.x && max.x >= other.max.x) &&
        (min.y <= other.min.y && max.y >= other.max.y)
    );
}

bool
AABB2::contains(const RectShape& rect) const {
    return contains(AABB2(rect));
}

bool
AABB2::contains(const CircleShape& circle) const {
    return contains(AABB2(circle));
}

bool
AABB2::contains(const LineShape& line) const {
    return contains(AABB2(line));
}

AABB3::AABB3(const Vec3& pos, const Vec3& size) : min(pos), max(pos + size) {}

bool
AABB3::intersect(const AABB3& other) const {
    return (
        (min.x <= other.max.x && max.x >= other.min.x) &&
        (min.y <= other.max.y && max.y >= other.min.y) &&
        (min.z <= other.max.z && max.z >= other.min.z)
    );
}

bool
AABB3::contains(const AABB3& other) const {
    return (
        (min.x <= other.min.x && max.x >= other.max.x) &&
        (min.y <= other.min.y && max.y >= other.max.y) &&
        (min.z <= other.min.z && max.z >= other.max.z)
    );
}

}  // namespace lili
