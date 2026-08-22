#include "lili2d/physics/circle_collider.hpp"

#include <algorithm>
#include <cmath>

#include "lili2d/physics/aabb_collider.hpp"

namespace lili {

CircleCollider::CircleCollider(const Vec2& center, float radius)
    : center(center), radius(radius) {}

CircleCollider::CircleCollider(const CircleShape& circle)
    : center(circle.center), radius(circle.radius) {}

CircleCollider::CircleCollider(const RectShape& rect)
    : center(Vec2(rect.x + rect.w * 0.5f, rect.y + rect.h * 0.5f)),
      radius(std::min(rect.w, rect.h) * 0.5f) {}

bool
CircleCollider::intersect(const CircleCollider& other) const {
    Vec2 diff = center - other.center;
    float r_sum = radius + other.radius;
    return diff.dot(diff) <= r_sum * r_sum;
}

bool
CircleCollider::intersect(const AABB2& aabb) const {
    float closest_x = std::clamp(center.x, aabb.min.x, aabb.max.x);
    float closest_y = std::clamp(center.y, aabb.min.y, aabb.max.y);
    Vec2 closest(closest_x, closest_y);
    Vec2 diff = center - closest;
    return diff.dot(diff) <= radius * radius;
}

bool
CircleCollider::intersect(const RectShape& rect) const {
    return intersect(AABB2(rect));
}

bool
CircleCollider::intersect(const LineShape& line) const {
    Vec2 start = line.start;
    Vec2 end = line.end;
    Vec2 seg = end - start;
    float seg_len_sq = seg.dot(seg);
    if (seg_len_sq == 0.0f) {
        return contains(start);
    }
    float t = ((center.x - start.x) * seg.x + (center.y - start.y) * seg.y) /
              seg_len_sq;
    t = std::clamp(t, 0.0f, 1.0f);
    Vec2 closest = start + seg * t;
    Vec2 diff = center - closest;
    return diff.dot(diff) <= radius * radius;
}

bool
CircleCollider::contains(const Vec2& point) const {
    Vec2 diff = point - center;
    return diff.dot(diff) <= radius * radius;
}

bool
CircleCollider::contains(const CircleCollider& other) const {
    if (other.radius > radius) return false;
    Vec2 diff = center - other.center;
    float max_dist = radius - other.radius;
    return diff.dot(diff) <= max_dist * max_dist;
}

AABB2
CircleCollider::getAABB() const {
    return AABB2(
        center - Vec2(radius, radius), Vec2(radius * 2.0f, radius * 2.0f)
    );
}

}  // namespace lili
