#pragma once

#include <cmath>

#include "lili2d/geometry/vec2.hpp"

namespace lili {

/// @brief Represents a 3x3 matrix for 2D transformations.
struct Mat3 {
    float m[9] = {};  ///< The matrix elements in column-major order.

    /// @brief Multiplies this matrix by another.
    /// @param other The matrix to multiply by.
    /// @return The resulting matrix.
    [[nodiscard]] constexpr Mat3
    operator*(const Mat3& other) const noexcept {
        Mat3 result{};

        for (int col = 0; col < 3; ++col)
            for (int row = 0; row < 3; ++row)
                result.m[col * 3 + row] =
                    (m[0 * 3 + row] * other.m[col * 3 + 0] +
                     m[1 * 3 + row] * other.m[col * 3 + 1] +
                     m[2 * 3 + row] * other.m[col * 3 + 2]);
        return result;
    }

    /// @brief Transforms a 2D point (x, y, 1) by this matrix.
    /// @param point The 2D point to transform.
    /// @return The transformed point.
    [[nodiscard]] inline Vec2
    transformPoint(Vec2 point) const noexcept {
        float x = m[0] * point.x + m[3] * point.y + m[6];
        float y = m[1] * point.x + m[4] * point.y + m[7];
        float w = m[2] * point.x + m[5] * point.y + m[8];

        if (std::abs(w) > 1e-6f && std::abs(w - 1.0f) > 1e-6f) {
            float inv_w = 1.0f / w;
            return {x * inv_w, y * inv_w};
        }
        return {x, y};
    }

    /// @brief Transforms a 2D direction vector (x, y, 0) by this matrix.
    /// @param vector The 2D direction vector to transform.
    /// @return The transformed vector.
    [[nodiscard]] constexpr Vec2
    transformVector(Vec2 vector) const noexcept {
        float x = m[0] * vector.x + m[3] * vector.y;
        float y = m[1] * vector.x + m[4] * vector.y;
        return {x, y};
    }

    /// @brief Calculates the determinant of the matrix.
    /// @return The determinant value.
    [[nodiscard]] constexpr float
    determinant() const noexcept {
        return m[0] * (m[4] * m[8] - m[7] * m[5]) -
               m[3] * (m[1] * m[8] - m[7] * m[2]) +
               m[6] * (m[1] * m[5] - m[4] * m[2]);
    }

    /// @brief Creates an identity matrix.
    /// @return The identity matrix.
    [[nodiscard]] static constexpr Mat3
    identity() noexcept {
        Mat3 result{};
        result.m[0] = 1.0f;
        result.m[4] = 1.0f;
        result.m[8] = 1.0f;
        return result;
    }

    /// @brief Calculates the inverse of the matrix.
    /// @return The inverse matrix. If not invertible, returns identity matrix.
    [[nodiscard]] inline Mat3
    inverse() const noexcept {
        float det = determinant();
        if (std::abs(det) < 1e-8f) return identity();

        float inv_det = 1.0f / det;

        Mat3 result{};
        result.m[0] = (m[4] * m[8] - m[7] * m[5]) * inv_det;
        result.m[1] = -(m[1] * m[8] - m[7] * m[2]) * inv_det;
        result.m[2] = (m[1] * m[5] - m[4] * m[2]) * inv_det;

        result.m[3] = -(m[3] * m[8] - m[6] * m[5]) * inv_det;
        result.m[4] = (m[0] * m[8] - m[6] * m[2]) * inv_det;
        result.m[5] = -(m[0] * m[5] - m[3] * m[2]) * inv_det;

        result.m[6] = (m[3] * m[7] - m[6] * m[4]) * inv_det;
        result.m[7] = -(m[0] * m[7] - m[6] * m[1]) * inv_det;
        result.m[8] = (m[0] * m[4] - m[3] * m[1]) * inv_det;

        return result;
    }

    /// @brief Calculates the transpose of the matrix.
    /// @return The transposed matrix.
    [[nodiscard]] constexpr Mat3
    transpose() const noexcept {
        Mat3 result{};
        for (int col = 0; col < 3; ++col)
            for (int row = 0; row < 3; ++row)
                result.m[row * 3 + col] = m[col * 3 + row];
        return result;
    }

    /// @brief Creates a 2D orthographic projection matrix.
    /// @param left The left clipping plane.
    /// @param right The right clipping plane.
    /// @param bottom The bottom clipping plane.
    /// @param top The top clipping plane.
    /// @return The orthographic projection matrix.
    [[nodiscard]] static constexpr Mat3
    orthographic(float left, float right, float bottom, float top) noexcept {
        Mat3 result{};
        result.m[0] = 2.0f / (right - left);
        result.m[4] = 2.0f / (bottom - top);
        result.m[6] = -(right + left) / (right - left);
        result.m[7] = -(bottom + top) / (bottom - top);
        result.m[8] = 1.0f;
        return result;
    }

    /// @brief Creates a 2D translation matrix.
    /// @param pos The translation vector.
    /// @return The translation matrix.
    [[nodiscard]] static constexpr Mat3
    translate(Vec2 pos) noexcept {
        Mat3 result = identity();
        result.m[6] = pos.x;
        result.m[7] = pos.y;
        return result;
    }

    /// @brief Creates a 2D rotation matrix.
    /// @param radians The rotation angle in radians.
    /// @return The rotation matrix.
    [[nodiscard]] static inline Mat3
    rotation(float radians) noexcept {
        Mat3 result = identity();
        float c = std::cos(radians);
        float s = std::sin(radians);

        result.m[0] = c;
        result.m[1] = s;
        result.m[3] = -s;
        result.m[4] = c;
        return result;
    }

    /// @brief Creates a 2D scaling matrix.
    /// @param v The scaling vector.
    /// @return The scaling matrix.
    [[nodiscard]] static constexpr Mat3
    scale(Vec2 v) noexcept {
        Mat3 result = identity();
        result.m[0] = v.x;
        result.m[4] = v.y;
        return result;
    }
};

}  // namespace lili
