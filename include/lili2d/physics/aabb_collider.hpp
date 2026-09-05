#pragma once

#include <algorithm>

#include "lili2d/geometry/shapes2d.hpp"
#include "lili2d/geometry/vec2.hpp"
#include "lili2d/geometry/vec3.hpp"

namespace lili {

struct CircleCollider;

/// @brief Represents an Axis-Aligned Bounding Box.
struct AABB2 {
    Vec2 min;  ///< The minimum coordinates.
    Vec2 max;  ///< The maximum coordinates.

    /// @brief Default constructor.
    constexpr AABB2() noexcept = default;
    /// @brief Copy constructor.
    constexpr AABB2(const AABB2&) noexcept = default;
    /// @brief Move constructor.
    constexpr AABB2(AABB2&&) noexcept = default;

    /// @brief Construct AABB2 with two Vec2.
    /// @param pos The position of the topleft.
    /// @param size The size of the bounding rect.
    constexpr AABB2(Vec2 pos, Vec2 size) noexcept : min(pos), max(pos + size) {}

    /// @brief Construct AABB2 with a RectShape.
    /// @param rect The shape of the bounding rect.
    constexpr explicit AABB2(RectShape rect) noexcept
        : min(rect.pos), max(rect.pos + rect.size) {}

    /// @brief Construct AABB2 with a CircleShape.
    /// @param circle The shape of the bounding circle.
    constexpr explicit AABB2(CircleShape circle) noexcept
        : min(circle.center - Vec2(circle.radius, circle.radius)),
          max(circle.center + Vec2(circle.radius, circle.radius)) {}

    /// @brief Construct AABB2 with a LineShape.
    /// @param line The shape of the bounding line.
    inline explicit AABB2(LineShape line) noexcept {
        min = Vec2(
            std::min(line.start.x, line.end.x),
            std::min(line.start.y, line.end.y)
        );
        max = Vec2(
            std::max(line.start.x, line.end.x),
            std::max(line.start.y, line.end.y)
        );
    }

    /// @brief Copy assignment operator.
    /// @return Reference to the assigned rectangle.
    constexpr AABB2&
    operator=(const AABB2&) noexcept = default;
    /// @brief Move assignment operator.
    /// @return Reference to the assigned rectangle.
    constexpr AABB2&
    operator=(AABB2&&) noexcept = default;

    /// @brief Checks if this AABB2 intersects with another.
    /// @param other The other AABB2 to test against.
    /// @return True if the AABB2s intersect, false otherwise.
    [[nodiscard]] constexpr bool
    intersect(AABB2 other) const noexcept {
        return (min.x <= other.max.x && max.x >= other.min.x) &&
               (min.y <= other.max.y && max.y >= other.min.y);
    }

    /// @brief Checks if this AABB2 intersects with a RectShape.
    /// @param rect The RectShape to test against.
    /// @return True if there is an intersection, false otherwise.
    [[nodiscard]] constexpr bool
    intersect(RectShape rect) const noexcept {
        return intersect(AABB2(rect));
    }

    /// @brief Checks if this AABB2 intersects with a CircleCollider.
    /// @param circle The CircleCollider to test against.
    /// @return True if there is an intersection, false otherwise.
    [[nodiscard]] bool
    intersect(CircleCollider circle) const noexcept;

    /// @brief Checks if this AABB2 intersects with a CircleShape.
    /// @param circle The CircleShape to test against.
    /// @return True if there is an intersection, false otherwise.
    [[nodiscard]] bool
    intersect(CircleShape circle) const noexcept;

    /// @brief Checks if this AABB2 contains another.
    /// @param other The other AABB2 to test against.
    /// @return True if this AABB2 contains the other, false otherwise.
    [[nodiscard]] constexpr bool
    contains(AABB2 other) const noexcept {
        return (min.x <= other.min.x && max.x >= other.max.x) &&
               (min.y <= other.min.y && max.y >= other.max.y);
    }

    /// @brief Checks if this AABB2 contains a RectShape.
    /// @param rect The RectShape to test against.
    /// @return True if this AABB2 contains the RectShape, false otherwise.
    [[nodiscard]] constexpr bool
    contains(RectShape rect) const noexcept {
        return contains(AABB2(rect));
    }

    /// @brief Checks if this AABB2 contains a CircleShape.
    /// @param circle The CircleShape to test against.
    /// @return True if this AABB2 contains the CircleShape, false otherwise.
    [[nodiscard]] constexpr bool
    contains(CircleShape circle) const noexcept {
        return contains(AABB2(circle));
    }

    /// @brief Checks if this AABB2 contains a LineShape.
    /// @param line The LineShape to test against.
    /// @return True if this AABB2 contains the LineShape, false otherwise.
    [[nodiscard]] inline bool
    contains(LineShape line) const noexcept {
        return contains(AABB2(line));
    }

    /// @brief Gets the RectShape corresponding to this AABB2.
    /// @return The rectangle shape.
    [[nodiscard]] constexpr RectShape
    getShape() const noexcept {
        return RectShape(min, max - min);
    }
};

/// @brief Represents a 3D Axis-Aligned Bounding Box.
struct AABB3 {
    Vec3 min;  ///< The minimum coordinates.
    Vec3 max;  ///< The maximum coordinates.

    /// @brief Default constructor.
    constexpr AABB3() noexcept = default;
    /// @brief Copy constructor.
    constexpr AABB3(const AABB3&) noexcept = default;
    /// @brief Move constructor.
    constexpr AABB3(AABB3&&) noexcept = default;

    /// @brief Construct AABB3 with two Vec3.
    /// @param pos The position of the min corner.
    /// @param size The size of the bounding box.
    constexpr AABB3(Vec3 pos, Vec3 size) noexcept : min(pos), max(pos + size) {}

    /// @brief Copy assignment operator.
    /// @return Reference to the assigned box.
    constexpr AABB3&
    operator=(const AABB3&) noexcept = default;
    /// @brief Move assignment operator.
    /// @return Reference to the assigned box.
    constexpr AABB3&
    operator=(AABB3&&) noexcept = default;

    /// @brief Checks if this AABB3 intersects with another.
    /// @param other The other AABB3 to test against.
    /// @return True if the AABB3s intersect, false otherwise.
    [[nodiscard]] constexpr bool
    intersect(AABB3 other) const noexcept {
        return (min.x <= other.max.x && max.x >= other.min.x) &&
               (min.y <= other.max.y && max.y >= other.min.y) &&
               (min.z <= other.max.z && max.z >= other.min.z);
    }

    /// @brief Checks if this AABB3 contains another.
    /// @param other The other AABB3 to test against.
    /// @return True if this AABB3 contains the other, false otherwise.
    [[nodiscard]] constexpr bool
    contains(AABB3 other) const noexcept {
        return (min.x <= other.min.x && max.x >= other.max.x) &&
               (min.y <= other.min.y && max.y >= other.max.y) &&
               (min.z <= other.min.z && max.z >= other.max.z);
    }
};

}  // namespace lili
