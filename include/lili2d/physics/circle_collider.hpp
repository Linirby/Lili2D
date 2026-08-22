#pragma once

#include "lili2d/geometry/shapes2d.hpp"
#include "lili2d/geometry/vec2.hpp"
#include "lili2d/geometry/vec4.hpp"

namespace lili {

struct AABB2;

/// @brief Represents a 2D Circle Collider for precise circle-based collision
/// detection.
struct CircleCollider {
    Vec2 center;          ///< Center position of the circle.
    float radius = 0.0f;  ///< Radius of the circle.

    /// @brief Default constructor.
    CircleCollider() = default;

    /// @brief Copy constructor.
    CircleCollider(const CircleCollider&) = default;

    /// @brief Move constructor.
    CircleCollider(CircleCollider&&) = default;

    /// @brief Constructs a CircleCollider from center and radius.
    /// @param center Center position.
    /// @param radius Circle radius.
    CircleCollider(const Vec2& center, float radius);

    /// @brief Constructs a CircleCollider from a geometry CircleShape.
    /// @param circle The circle shape.
    CircleCollider(const CircleShape& circle);

    /// @brief Constructs a CircleCollider from a geometry RectShape.
    /// @param rect The rectangle shape.
    CircleCollider(const RectShape& rect);

    /// @brief Copy assignment operator.
    CircleCollider&
    operator=(const CircleCollider&) = default;

    /// @brief Move assignment operator.
    CircleCollider&
    operator=(CircleCollider&&) = default;

    /// @brief Checks if this circle intersects with another circle collider.
    /// @param other The other circle collider.
    /// @return True if there is an intersection, false otherwise.
    bool
    intersect(const CircleCollider& other) const;

    /// @brief Checks if this circle intersects with an AABB2.
    /// @param aabb The AABB2 to check against.
    /// @return True if there is an intersection, false otherwise.
    bool
    intersect(const AABB2& aabb) const;

    /// @brief Checks if this circle intersects with a RectShape.
    /// @param rect The rect to check against.
    /// @return True if there is an intersection, false otherwise.
    bool
    intersect(const RectShape& rect) const;

    /// @brief Checks if this circle intersects with a LineShape segment.
    /// @param line The line segment to check against.
    /// @return True if there is an intersection, false otherwise.
    bool
    intersect(const LineShape& line) const;

    /// @brief Checks if a point is contained inside this circle.
    /// @param point The point to test.
    /// @return True if point is inside, false otherwise.
    bool
    contains(const Vec2& point) const;

    /// @brief Checks if another circle is completely contained inside this
    /// circle.
    /// @param other The other circle collider.
    /// @return True if other circle is fully contained, false otherwise.
    bool
    contains(const CircleCollider& other) const;

    /// @brief Gets the Axis-Aligned Bounding Box (AABB2) surrounding this
    /// circle.
    /// @return Bounding AABB2.
    AABB2
    getAABB() const;
};

}  // namespace lili
