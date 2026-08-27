#pragma once

namespace lili {

/// @brief Represents a 3D mathematical point.
struct Point3 {
    int x = 0;  ///< X coordinate.
    int y = 0;  ///< Y coordinate.
    int z = 0;  ///< Z coordinate.

    /// @brief Default constructor.
    constexpr Point3() noexcept = default;
    /// @brief Constructs a 3D point with given values.
    /// @param x The X component.
    /// @param y The Y component.
    /// @param z The Z component.
    constexpr Point3(int x, int y, int z) noexcept : x(x), y(y), z(z) {}

    /// @brief Unary negation operator.
    /// @return The negated point.
    [[nodiscard]] constexpr Point3
    operator-() const noexcept {
        return {-x, -y, -z};
    }

    /// @brief Subtraction operator.
    /// @param o The point to subtract.
    /// @return The result of subtraction.
    [[nodiscard]] constexpr Point3
    operator-(Point3 o) const noexcept {
        return {x - o.x, y - o.y, z - o.z};
    }

    /// @brief Subtraction assignment operator.
    /// @param o The point to subtract.
    /// @return A reference to this point.
    constexpr Point3&
    operator-=(Point3 o) noexcept {
        x -= o.x;
        y -= o.y;
        z -= o.z;
        return *this;
    }

    /// @brief Addition operator.
    /// @param o The point to add.
    /// @return The result of addition.
    [[nodiscard]] constexpr Point3
    operator+(Point3 o) const noexcept {
        return {x + o.x, y + o.y, z + o.z};
    }

    /// @brief Addition assignment operator.
    /// @param o The point to add.
    /// @return A reference to this point.
    constexpr Point3&
    operator+=(Point3 o) noexcept {
        x += o.x;
        y += o.y;
        z += o.z;
        return *this;
    }

    /// @brief Scalar multiplication operator.
    /// @param scalar The scalar value.
    /// @return The scaled point.
    [[nodiscard]] constexpr Point3
    operator*(int scalar) const noexcept {
        return {x * scalar, y * scalar, z * scalar};
    }

    /// @brief Scalar multiplication assignment operator.
    /// @param scalar The scalar value.
    /// @return A reference to this point.
    constexpr Point3&
    operator*=(float scalar) noexcept {
        x = static_cast<int>(x * scalar);
        y = static_cast<int>(y * scalar);
        z = static_cast<int>(z * scalar);
        return *this;
    }

    /// @brief Is equal assignment operator.
    /// @param o The point to check with.
    /// @return True if they're equal otherwise false.
    [[nodiscard]] constexpr bool
    operator==(Point3 o) const noexcept {
        return x == o.x && y == o.y && z == o.z;
    }
};

}  // namespace lili
