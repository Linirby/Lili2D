#pragma once

namespace lili {

/// @brief Represents a 2D mathematical point.
struct Point2 {
    int x = 0;  ///< X coordinate.
    int y = 0;  ///< Y coordinate.

    /// @brief Default constructor.
    constexpr Point2() noexcept = default;
    /// @brief Constructs a 2D point with given values.
    /// @param x The X component.
    /// @param y The Y component.
    constexpr Point2(int x, int y) noexcept : x(x), y(y) {}

    /// @brief Unary negation operator.
    /// @return The negated point.
    [[nodiscard]] constexpr Point2
    operator-() const noexcept {
        return {-x, -y};
    }

    /// @brief Subtraction operator.
    /// @param o The point to subtract.
    /// @return The result of subtraction.
    [[nodiscard]] constexpr Point2
    operator-(Point2 o) const noexcept {
        return {x - o.x, y - o.y};
    }

    /// @brief Subtraction assignment operator.
    /// @param o The point to subtract.
    /// @return A reference to this point.
    constexpr Point2&
    operator-=(Point2 o) noexcept {
        x -= o.x;
        y -= o.y;
        return *this;
    }

    /// @brief Addition operator.
    /// @param o The point to add.
    /// @return The result of addition.
    [[nodiscard]] constexpr Point2
    operator+(Point2 o) const noexcept {
        return {x + o.x, y + o.y};
    }

    /// @brief Addition assignment operator.
    /// @param o The point to add.
    /// @return A reference to this point.
    constexpr Point2&
    operator+=(Point2 o) noexcept {
        x += o.x;
        y += o.y;
        return *this;
    }

    /// @brief Scalar multiplication operator.
    /// @param scalar The scalar value.
    /// @return The scaled point.
    [[nodiscard]] constexpr Point2
    operator*(int scalar) const noexcept {
        return {x * scalar, y * scalar};
    }

    /// @brief Scalar multiplication assignment operator.
    /// @param scalar The scalar value.
    /// @return A reference to this point.
    constexpr Point2&
    operator*=(float scalar) noexcept {
        x = static_cast<int>(x * scalar);
        y = static_cast<int>(y * scalar);
        return *this;
    }

    /// @brief Is equal assignment operator.
    /// @param o The point to check with.
    /// @return True if they're equal otherwise false.
    [[nodiscard]] constexpr bool
    operator==(Point2 o) const noexcept {
        return x == o.x && y == o.y;
    }
};

}  // namespace lili
