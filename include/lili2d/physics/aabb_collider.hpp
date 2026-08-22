#pragma once

#include "lili2d/geometry/shapes2d.hpp"
#include "lili2d/geometry/vec2.hpp"
#include "lili2d/geometry/vec3.hpp"

namespace lili {

struct CircleCollider;

/// @brief Represents an Axis-Aligned Bounding Box.
struct AABB2 {
    /// @brief Default constructor.
    AABB2() = default;
    /// @brief Copy constructor.
    AABB2(const AABB2&) = default;
    /// @brief Move constructor.
    AABB2(AABB2&&) = default;
    /// @brief Construct AABB2 with two Vec2.
    /// @param pos The position of the topleft.
    /// @param size The size of the bounding rect.
    AABB2(const Vec2& pos, const Vec2& size);
    /// @brief Construct AABB2 with a RectShape.
    /// @param rect The shape of the bounding rect.
    AABB2(const RectShape& rect);
    /// @brief Construct AABB2 with a CircleShape.
    /// @param circle The shape of the bounding circle.
    AABB2(const CircleShape& circle);
    /// @brief Construct AABB2 with a LineShape.
    /// @param line The shape of the bounding line.
    AABB2(const LineShape& line);
    /// @brief Copy assignment operator.
    /// @return Reference to the assigned rectangle.
    AABB2&
    operator=(const AABB2&) = default;
    /// @brief Move assignment operator.
    /// @return Reference to the assigned rectangle.
    AABB2&
    operator=(AABB2&&) = default;

    Vec2 min;  ///< The minimum coordinates.
    Vec2 max;  ///< The maximum coordinates.

    /// @brief Checks if this AABB2 intersects with another.
    /// @param other The other AABB2 to test against.
    /// @return True if the AABB2s intersect, false otherwise.
    bool
    intersect(const AABB2& other) const;
    /// @brief Checks if this AABB2 intersects with a RectShape.
    /// @param rect The RectShape to test against.
    /// @return True if there is an intersection, false otherwise.
    bool
    intersect(const RectShape& rect) const;
    /// @brief Checks if this AABB2 intersects with a CircleCollider.
    /// @param circle The CircleCollider to test against.
    /// @return True if there is an intersection, false otherwise.
    bool
    intersect(const CircleCollider& circle) const;
    /// @brief Checks if this AABB2 intersects with a CircleShape.
    /// @param circle The CircleShape to test against.
    /// @return True if there is an intersection, false otherwise.
    bool
    intersect(const CircleShape& circle) const;

    /// @brief Checks if this AABB2 contains another.
    /// @param other The other AABB2 to test against.
    /// @return True if this AABB2 contains the other, false otherwise.
    bool
    contains(const AABB2& other) const;
    /// @brief Checks if this AABB2 contains a RectShape.
    /// @param rect The RectShape to test against.
    /// @return True if this AABB2 contains the RectShape, false otherwise.
    bool
    contains(const RectShape& rect) const;
    /// @brief Checks if this AABB2 contains a CircleShape.
    /// @param circle The CircleShape to test against.
    /// @return True if this AABB2 contains the CircleShape, false otherwise.
    bool
    contains(const CircleShape& circle) const;
    /// @brief Checks if this AABB2 contains a LineShape.
    /// @param line The LineShape to test against.
    /// @return True if this AABB2 contains the LineShape, false otherwise.
    bool
    contains(const LineShape& line) const;
};

/// @brief Represents a 3D Axis-Aligned Bounding Box.
struct AABB3 {
    /// @brief Default constructor.
    AABB3() = default;
    /// @brief Copy constructor.
    AABB3(const AABB3&) = default;
    /// @brief Move constructor.
    AABB3(AABB3&&) = default;
    /// @brief Construct AABB3 with two Vec3.
    /// @param pos The position of the min corner.
    /// @param size The size of the bounding box.
    AABB3(const Vec3& pos, const Vec3& size);
    /// @brief Copy assignment operator.
    /// @return Reference to the assigned box.
    AABB3&
    operator=(const AABB3&) = default;
    /// @brief Move assignment operator.
    /// @return Reference to the assigned box.
    AABB3&
    operator=(AABB3&&) = default;

    Vec3 min;  ///< The minimum coordinates.
    Vec3 max;  ///< The maximum coordinates.

    /// @brief Checks if this AABB3 intersects with another.
    /// @param other The other AABB3 to test against.
    /// @return True if the AABB3s intersect, false otherwise.
    bool
    intersect(const AABB3& other) const;

    /// @brief Checks if this AABB3 contains another.
    /// @param other The other AABB3 to test against.
    /// @return True if this AABB3 contains the other, false otherwise.
    bool
    contains(const AABB3& other) const;
};

}  // namespace lili
