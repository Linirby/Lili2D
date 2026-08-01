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
    RectShape() = default;
    /// @brief Copy constructor.
    RectShape(const RectShape&) = default;
    /// @brief Constructs a rectangle shape.
    /// @param x X coordinate.
    /// @param y Y coordinate.
    /// @param w Width.
    /// @param h Height.
    RectShape(float x, float y, float w, float h) : x(x), y(y), w(w), h(h) {}

    /// @brief Checks if a point is contained inside this rectangle.
    /// @param point The 2D point to test.
    /// @return True if point is inside.
    bool
    contains(const Vec2& point) const {
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
    CircleShape() = default;
    /// @brief Copy constructor.
    CircleShape(const CircleShape&) = default;
    /// @brief Constructs a circle shape.
    /// @param center The center position.
    /// @param radius The radius.
    /// @param segments The number of segments.
    CircleShape(Vec2 center, float radius, int segments = 16)
        : center(center), radius(radius), segments(segments) {}

    /// @brief Checks if a point is contained inside this circle.
    /// @param point The 2D point to test.
    /// @return True if point is inside circle radius.
    bool
    contains(const Vec2& point) const {
        return (point - center).length() <= radius;
    }
};

/// @brief Defines the geometry of a 2D line.
struct LineShape {
    Vec2 start = {0.0f, 0.0f};  ///< The start position.
    Vec2 end = {0.0f, 0.0f};    ///< The end position.
    float thickness = 1.0f;     ///< The thickness of the line.

    /// @brief Default constructor.
    LineShape() = default;
    /// @brief Copy constructor.
    LineShape(const LineShape&) = default;
    /// @brief Constructs a line shape.
    /// @param start The start position.
    /// @param end The end position.
    /// @param thickness The thickness.
    LineShape(Vec2 start, Vec2 end, float thickness = 1.0f)
        : start(start), end(end), thickness(thickness) {}
};

}  // namespace lili
