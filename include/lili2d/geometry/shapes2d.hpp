#pragma once

#include <cmath>

#include "lili2d/geometry/vec2.hpp"

namespace lili {

/// @brief Defines the geometry of a 2D rectangle.
struct RectShape {
    float x = 0.0f;  ///< X position.
    float y = 0.0f;  ///< Y position.
    float w = 0.0f;  ///< Width.
    float h = 0.0f;  ///< Height.

    /// @brief Default constructor.
    constexpr RectShape() noexcept = default;
    /// @brief Constructs a rectangle shape.
    /// @param x X coordinate.
    /// @param y Y coordinate.
    /// @param w Width.
    /// @param h Height.
    constexpr RectShape(float x, float y, float w, float h) noexcept
        : x(x), y(y), w(w), h(h) {}

    /// @brief Checks if a point is contained inside this rectangle.
    /// @param point The 2D point to test.
    /// @return True if point is inside.
    [[nodiscard]] constexpr bool
    contains(Vec2 point) const noexcept {
        return point.x >= x && point.x <= x + w && point.y >= y &&
               point.y <= y + h;
    }
};

/// @brief Defines the geometry of a 2D circle.
struct CircleShape {
    Vec2 center = {0.0f, 0.0f};  ///< The center position.
    float radius = 0.0f;         ///< The radius.
    int segments = 3;            ///< The number of segments.

    /// @brief Default constructor.
    constexpr CircleShape() noexcept = default;
    /// @brief Constructs a circle shape.
    /// @param center The center position.
    /// @param radius The radius.
    /// @param segments The number of segments.
    constexpr CircleShape(Vec2 center, float radius, int segments = 16) noexcept
        : center(center), radius(radius), segments(segments) {}

    /// @brief Checks if a point is contained inside this circle.
    /// @param point The 2D point to test.
    /// @return True if point is inside circle radius.
    [[nodiscard]] inline bool
    contains(Vec2 point) const noexcept {
        return (point - center).length() <= radius;
    }
};

/// @brief Defines the geometry of a 2D line.
struct LineShape {
    Vec2 start = {0.0f, 0.0f};  ///< The start position.
    Vec2 end = {0.0f, 0.0f};    ///< The end position.
    float thickness = 1.0f;     ///< The thickness of the line.

    /// @brief Default constructor.
    constexpr LineShape() noexcept = default;
    /// @brief Constructs a line shape.
    /// @param start The start position.
    /// @param end The end position.
    /// @param thickness The thickness.
    constexpr LineShape(Vec2 start, Vec2 end, float thickness = 1.0f) noexcept
        : start(start), end(end), thickness(thickness) {}
};

}  // namespace lili
