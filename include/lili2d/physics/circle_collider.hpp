#pragma once

#include <algorithm>
#include <cmath>

#include "lili2d/geometry/shapes2d.hpp"
#include "lili2d/geometry/vec2.hpp"
#include "lili2d/physics/aabb_collider.hpp"

namespace lili {

/// @brief Represents a 2D Circle Collider for precise circle-based collision
/// detection.
struct CircleCollider {
    Vec2 center = {};     ///< Center position of the circle.
    float radius = 0.0f;  ///< Radius of the circle.

    /// @brief Default constructor.
    constexpr CircleCollider() noexcept = default;

    /// @brief Copy constructor.
    constexpr CircleCollider(const CircleCollider&) noexcept = default;

    /// @brief Move constructor.
    constexpr CircleCollider(CircleCollider&&) noexcept = default;

    /// @brief Constructs a CircleCollider from center and radius.
    /// @param center Center position.
    /// @param radius Circle radius.
    constexpr CircleCollider(Vec2 center, float radius) noexcept
        : center(center), radius(radius) {}

    /// @brief Constructs a CircleCollider from a geometry CircleShape.
    /// @param circle The circle shape.
    constexpr explicit CircleCollider(CircleShape circle) noexcept
        : center(circle.center), radius(circle.radius) {}

    /// @brief Constructs a CircleCollider from a geometry RectShape.
    /// @param rect The rectangle shape.
    inline explicit CircleCollider(RectShape rect) noexcept
        : center(Vec2(rect.x + rect.w * 0.5f, rect.y + rect.h * 0.5f)),
          radius(std::min(rect.w, rect.h) * 0.5f) {}

    /// @brief Copy assignment operator.
    constexpr CircleCollider&
    operator=(const CircleCollider&) noexcept = default;

    /// @brief Move assignment operator.
    constexpr CircleCollider&
    operator=(CircleCollider&&) noexcept = default;

    /// @brief Checks if this circle intersects with another circle collider.
    /// @param other The other circle collider.
    /// @return True if there is an intersection, false otherwise.
    [[nodiscard]] constexpr bool
    intersect(CircleCollider other) const noexcept {
        Vec2 diff = center - other.center;
        float r_sum = radius + other.radius;
        return diff.dot(diff) <= r_sum * r_sum;
    }

    /// @brief Checks if this circle intersects with an AABB2.
    /// @param aabb The AABB2 to check against.
    /// @return True if there is an intersection, false otherwise.
    [[nodiscard]] inline bool
    intersect(AABB2 aabb) const noexcept {
        float closest_x = std::clamp(center.x, aabb.min.x, aabb.max.x);
        float closest_y = std::clamp(center.y, aabb.min.y, aabb.max.y);
        Vec2 closest(closest_x, closest_y);
        Vec2 diff = center - closest;
        return diff.dot(diff) <= radius * radius;
    }

    /// @brief Checks if this circle intersects with a RectShape.
    /// @param rect The rect to check against.
    /// @return True if there is an intersection, false otherwise.
    [[nodiscard]] inline bool
    intersect(RectShape rect) const noexcept {
        return intersect(AABB2(rect));
    }

    /// @brief Checks if this circle intersects with a LineShape segment.
    /// @param line The line segment to check against.
    /// @return True if there is an intersection, false otherwise.
    [[nodiscard]] inline bool
    intersect(LineShape line) const noexcept {
        Vec2 start = line.start;
        Vec2 end = line.end;
        Vec2 seg = end - start;
        float seg_len_sq = seg.dot(seg);
        if (seg_len_sq == 0.0f) return contains(start);
        float t =
            ((center.x - start.x) * seg.x + (center.y - start.y) * seg.y) /
            seg_len_sq;
        t = std::clamp(t, 0.0f, 1.0f);
        Vec2 closest = start + seg * t;
        Vec2 diff = center - closest;
        return diff.dot(diff) <= radius * radius;
    }

    /// @brief Checks if a point is contained inside this circle.
    /// @param point The point to test.
    /// @return True if point is inside, false otherwise.
    [[nodiscard]] constexpr bool
    contains(Vec2 point) const noexcept {
        Vec2 diff = point - center;
        return diff.dot(diff) <= radius * radius;
    }

    /// @brief Checks if another circle is completely contained inside this
    /// circle.
    /// @param other The other circle collider.
    /// @return True if other circle is fully contained, false otherwise.
    [[nodiscard]] constexpr bool
    contains(CircleCollider other) const noexcept {
        if (other.radius > radius) return false;
        Vec2 diff = center - other.center;
        float max_dist = radius - other.radius;
        return diff.dot(diff) <= max_dist * max_dist;
    }

    /// @brief Gets the Axis-Aligned Bounding Box (AABB2) surrounding this
    /// circle.
    /// @return Bounding AABB2.
    [[nodiscard]] constexpr AABB2
    getAABB() const noexcept {
        return AABB2(
            center - Vec2(radius, radius), Vec2(radius * 2.0f, radius * 2.0f)
        );
    }
};

inline bool
AABB2::intersect(CircleCollider circle) const noexcept {
    return circle.intersect(*this);
}

inline bool
AABB2::intersect(CircleShape circle) const noexcept {
    return CircleCollider(circle).intersect(*this);
}

}  // namespace lili
