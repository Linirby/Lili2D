#pragma once

#include <cmath>

namespace lili {

/// @brief Represents a 2D mathematical vector.
struct Vec2 {
    float x = 0.0f;  ///< X coordinate.
    float y = 0.0f;  ///< Y coordinate.

    /// @brief Default constructor.
    constexpr Vec2() noexcept = default;
    /// @brief Constructs a 2D vector with given values.
    /// @param x The X component.
    /// @param y The Y component.
    constexpr Vec2(float x, float y) noexcept : x(x), y(y) {}

    /// @brief Unary negation operator.
    /// @return The negated vector.
    [[nodiscard]] constexpr Vec2
    operator-() const noexcept {
        return {-x, -y};
    }

    /// @brief Subtraction operator.
    /// @param o The vector to subtract.
    /// @return The result of subtraction.
    [[nodiscard]] constexpr Vec2
    operator-(Vec2 o) const noexcept {
        return {x - o.x, y - o.y};
    }

    /// @brief Subtraction assignment operator.
    /// @param o The vector to subtract.
    /// @return A reference to this vector.
    constexpr Vec2&
    operator-=(Vec2 o) noexcept {
        x -= o.x;
        y -= o.y;
        return *this;
    }

    /// @brief Addition operator.
    /// @param o The vector to add.
    /// @return The result of addition.
    [[nodiscard]] constexpr Vec2
    operator+(Vec2 o) const noexcept {
        return {x + o.x, y + o.y};
    }

    /// @brief Addition assignment operator.
    /// @param o The vector to add.
    /// @return A reference to this vector.
    constexpr Vec2&
    operator+=(Vec2 o) noexcept {
        x += o.x;
        y += o.y;
        return *this;
    }

    /// @brief Scalar multiplication operator.
    /// @param scalar The scalar value.
    /// @return The scaled vector.
    [[nodiscard]] constexpr Vec2
    operator*(float scalar) const noexcept {
        return {x * scalar, y * scalar};
    }

    /// @brief Scalar multiplication assignment operator.
    /// @param scalar The scalar value.
    /// @return A reference to this vector.
    constexpr Vec2&
    operator*=(float scalar) noexcept {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    /// @brief Is equal assignment operator.
    /// @param o The vector to check with.
    /// @return True if they're equal otherwise false.
    [[nodiscard]] constexpr bool
    operator==(Vec2 o) const noexcept {
        return x == o.x && y == o.y;
    }

    /// @brief Calculates the dot product with another vector.
    /// @param o The other vector.
    /// @return The dot product.
    [[nodiscard]] constexpr float
    dot(Vec2 o) const noexcept {
        return x * o.x + y * o.y;
    }

    /// @brief Calculates the 2D cross product with another vector.
    /// @param o The other vector.
    /// @return The cross product scalar value.
    [[nodiscard]] constexpr float
    cross(Vec2 o) const noexcept {
        return x * o.y - y * o.x;
    }

    /// @brief Returns a normalized copy of this vector.
    /// @return The normalized vector.
    [[nodiscard]] inline Vec2
    normalized() const noexcept {
        float sq_len = x * x + y * y;
        if (sq_len == 0.0f) {
            return {0.0f, 0.0f};
        }
        return *this * (1.0f / std::sqrt(sq_len));
    }

    /// @brief Linearly interpolates between this and another vector.
    /// @param o The target vector.
    /// @param t Interpolation factor [0.0, 1.0].
    /// @return The interpolated vector.
    [[nodiscard]] constexpr Vec2
    lerp(Vec2 o, float t) const noexcept {
        return {std::lerp(x, o.x, t), std::lerp(y, o.y, t)};
    }

    /// @brief Gets the length (magnitude) of the vector.
    /// @return The length.
    [[nodiscard]] inline float
    length() const noexcept {
        return std::sqrt(x * x + y * y);
    }
};

}  // namespace lili
