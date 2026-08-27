#pragma once

#include <cmath>

namespace lili {

/// @brief Represents a 4D mathematical vector.
struct Vec4 {
    float x = 0.0f;  ///< X coordinate.
    float y = 0.0f;  ///< Y coordinate.
    float z = 0.0f;  ///< Z coordinate.
    float w = 0.0f;  ///< W coordinate.

    /// @brief Default constructor.
    constexpr Vec4() noexcept = default;
    /// @brief Constructs a 4D vector with given values.
    /// @param x The X component.
    /// @param y The Y component.
    /// @param z The Z component.
    /// @param w The W component.
    constexpr Vec4(float x, float y, float z, float w) noexcept
        : x(x), y(y), z(z), w(w) {}

    /// @brief Unary negation operator.
    /// @return The negated vector.
    [[nodiscard]] constexpr Vec4
    operator-() const noexcept {
        return {-x, -y, -z, -w};
    }

    /// @brief Subtraction operator.
    /// @param o The vector to subtract.
    /// @return The result of subtraction.
    [[nodiscard]] constexpr Vec4
    operator-(Vec4 o) const noexcept {
        return {x - o.x, y - o.y, z - o.z, w - o.w};
    }

    /// @brief Subtraction assignment operator.
    /// @param o The vector to subtract.
    /// @return A reference to this vector.
    constexpr Vec4&
    operator-=(Vec4 o) noexcept {
        x -= o.x;
        y -= o.y;
        z -= o.z;
        w -= o.w;
        return *this;
    }

    /// @brief Addition operator.
    /// @param o The vector to add.
    /// @return The result of addition.
    [[nodiscard]] constexpr Vec4
    operator+(Vec4 o) const noexcept {
        return {x + o.x, y + o.y, z + o.z, w + o.w};
    }

    /// @brief Addition assignment operator.
    /// @param o The vector to add.
    /// @return A reference to this vector.
    constexpr Vec4&
    operator+=(Vec4 o) noexcept {
        x += o.x;
        y += o.y;
        z += o.z;
        w += o.w;
        return *this;
    }

    /// @brief Scalar multiplication operator.
    /// @param scalar The scalar value.
    /// @return The scaled vector.
    [[nodiscard]] constexpr Vec4
    operator*(float scalar) const noexcept {
        return {x * scalar, y * scalar, z * scalar, w * scalar};
    }

    /// @brief Scalar multiplication assignment operator.
    /// @param scalar The scalar value.
    /// @return A reference to this vector.
    constexpr Vec4&
    operator*=(float scalar) noexcept {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        w *= scalar;
        return *this;
    }

    /// @brief Is equal assignment operator.
    /// @param o The vector to check with.
    /// @return True if they're equal otherwise false.
    [[nodiscard]] constexpr bool
    operator==(Vec4 o) const noexcept {
        return x == o.x && y == o.y && z == o.z && w == o.w;
    }

    /// @brief Calculates the dot product with another vector.
    /// @param o The other vector.
    /// @return The dot product.
    [[nodiscard]] constexpr float
    dot(Vec4 o) const noexcept {
        return x * o.x + y * o.y + z * o.z + w * o.w;
    }

    /// @brief Calculates the cross product with another vector.
    /// @param o The other vector.
    /// @return The resulting cross product vector.
    [[nodiscard]] constexpr Vec4
    cross(Vec4 o) const noexcept {
        return {y * o.z - z * o.y, z * o.x - x * o.z, x * o.y - y * o.x, 0.0f};
    }

    /// @brief Returns a normalized copy of this vector.
    /// @return The normalized vector.
    [[nodiscard]] inline Vec4
    normalized() const noexcept {
        float sq_len = x * x + y * y + z * z + w * w;
        if (sq_len == 0.0f) {
            return {0.0f, 0.0f, 0.0f, 0.0f};
        }
        return *this * (1.0f / std::sqrt(sq_len));
    }

    /// @brief Gets the length (magnitude) of the vector.
    /// @return The length.
    [[nodiscard]] inline float
    length() const noexcept {
        return std::sqrt(x * x + y * y + z * z + w * w);
    }
};

}  // namespace lili
