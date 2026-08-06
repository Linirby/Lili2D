#pragma once

#include "lili2d/geometry/vec2.hpp"

namespace lili {

/// @brief Represents a 3x3 matrix for 2D transformations.
struct Mat3 {
    float m[9] = {};  ///< The matrix elements in column-major order.

    /// @brief Multiplies this matrix by another.
    /// @param other The matrix to multiply by.
    /// @return The resulting matrix.
    Mat3
    operator*(const Mat3& other) const;

    /// @brief Transforms a 2D point (x, y, 1) by this matrix.
    /// @param point The 2D point to transform.
    /// @return The transformed point.
    Vec2
    transformPoint(const Vec2& point) const;

    /// @brief Transforms a 2D direction vector (x, y, 0) by this matrix.
    /// @param vector The 2D direction vector to transform.
    /// @return The transformed vector.
    Vec2
    transformVector(const Vec2& vector) const;

    /// @brief Calculates the determinant of the matrix.
    /// @return The determinant value.
    float
    determinant() const;

    /// @brief Calculates the inverse of the matrix.
    /// @return The inverse matrix. If not invertible, returns identity matrix.
    Mat3
    inverse() const;

    /// @brief Calculates the transpose of the matrix.
    /// @return The transposed matrix.
    Mat3
    transpose() const;

    /// @brief Creates an identity matrix.
    /// @return The identity matrix.
    static Mat3
    identity();
    /// @brief Creates a 2D orthographic projection matrix.
    /// @param left The left clipping plane.
    /// @param right The right clipping plane.
    /// @param bottom The bottom clipping plane.
    /// @param top The top clipping plane.
    /// @return The orthographic projection matrix.
    static Mat3
    orthographic(float left, float right, float bottom, float top);
    /// @brief Creates a 2D translation matrix.
    /// @param pos The translation vector.
    /// @return The translation matrix.
    static Mat3
    translate(Vec2 pos);
    /// @brief Creates a 2D rotation matrix.
    /// @param radians The rotation angle in radians.
    /// @return The rotation matrix.
    static Mat3
    rotation(float radians);
    /// @brief Creates a 2D scaling matrix.
    /// @param v The scaling vector.
    /// @return The scaling matrix.
    static Mat3
    scale(Vec2 v);
};

}  // namespace lili
