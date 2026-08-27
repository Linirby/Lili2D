#pragma once

#include <cmath>

namespace lili {

/// @brief Represents a 3D mathematical vector.
struct Vec3 {
    float x = 0.0f;  ///< X coordinate.
    float y = 0.0f;  ///< Y coordinate.
    float z = 0.0f;  ///< Z coordinate.

    /// @brief Default constructor.
    constexpr Vec3() noexcept = default;
    /// @brief Constructs a 3D vector with given values.
    /// @param x The X component.
    /// @param y The Y component.
    /// @param z The Z component.
    constexpr Vec3(float x, float y, float z) noexcept : x(x), y(y), z(z) {}

    /// @brief Unary negation operator.
    /// @return The negated vector.
    [[nodiscard]] constexpr Vec3
    operator-() const noexcept {
        return {-x, -y, -z};
    }

    /// @brief Subtraction operator.
    /// @param o The vector to subtract.
    /// @return The result of subtraction.
    [[nodiscard]] constexpr Vec3
    operator-(Vec3 o) const noexcept {
        return {x - o.x, y - o.y, z - o.z};
    }

    /// @brief Subtraction assignment operator.
    /// @param o The vector to subtract.
    /// @return A reference to this vector.
    constexpr Vec3&
    operator-=(Vec3 o) noexcept {
        x -= o.x;
        y -= o.y;
        z -= o.z;
        return *this;
    }

    /// @brief Addition operator.
    /// @param o The vector to add.
    /// @return The result of addition.
    [[nodiscard]] constexpr Vec3
    operator+(Vec3 o) const noexcept {
        return {x + o.x, y + o.y, z + o.z};
    }

    /// @brief Addition assignment operator.
    /// @param o The vector to add.
    /// @return A reference to this vector.
    constexpr Vec3&
    operator+=(Vec3 o) noexcept {
        x += o.x;
        y += o.y;
        z += o.z;
        return *this;
    }

    /// @brief Scalar multiplication operator.
    /// @param scalar The scalar value.
    /// @return The scaled vector.
    [[nodiscard]] constexpr Vec3
    operator*(float scalar) const noexcept {
        return {x * scalar, y * scalar, z * scalar};
    }

    /// @brief Scalar multiplication assignment operator.
    /// @param scalar The scalar value.
    /// @return A reference to this vector.
    constexpr Vec3&
    operator*=(float scalar) noexcept {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }

    /// @brief Is equal assignment operator.
    /// @param o The vector to check with.
    /// @return True if they're equal otherwise false.
    [[nodiscard]] constexpr bool
    operator==(Vec3 o) const noexcept {
        return x == o.x && y == o.y && z == o.z;
    }

    /// @brief Calculates the dot product with another vector.
    /// @param o The other vector.
    /// @return The dot product.
    [[nodiscard]] constexpr float
    dot(Vec3 o) const noexcept {
        return x * o.x + y * o.y + z * o.z;
    }

    /// @brief Calculates the 3D cross product with another vector.
    /// @param o The other vector.
    /// @return The resulting cross product vector.
    [[nodiscard]] constexpr Vec3
    cross(Vec3 o) const noexcept {
        return {y * o.z - z * o.y, z * o.x - x * o.z, x * o.y - y * o.x};
    }

    /// @brief Returns a normalized copy of this vector.
    /// @return The normalized vector.
    [[nodiscard]] inline Vec3
    normalized() const noexcept {
        float sq_len = x * x + y * y + z * z;
        if (sq_len == 0.0f) {
            return {0.0f, 0.0f, 0.0f};
        }
        return *this * (1.0f / std::sqrt(sq_len));
    }

    /// @brief Linearly interpolates between this and another vector.
    /// @param o The target vector.
    /// @param t The interpolation factor.
    /// @return The interpolated vector.
    [[nodiscard]] constexpr Vec3
    lerp(Vec3 o, float t) const noexcept {
        return {
            std::lerp(x, o.x, t), std::lerp(y, o.y, t), std::lerp(z, o.z, t)
        };
    }

    /// @brief Gets the length (magnitude) of the vector.
    /// @return The length.
    [[nodiscard]] inline float
    length() const noexcept {
        return std::sqrt(x * x + y * y + z * z);
    }
};

}  // namespace lili
