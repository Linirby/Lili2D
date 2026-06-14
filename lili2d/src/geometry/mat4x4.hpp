#pragma once

#include "vec3.hpp"

namespace lili {

/// @brief Represents a 4x4 matrix for 3D transformations.
struct Mat4 {
	float m[16] = {};  ///< The matrix elements in column-major order.

	/// @brief Multiplies this matrix by another.
	/// @param other The matrix to multiply by.
	/// @return The resulting matrix.
	Mat4 operator*(const Mat4 &other) const;

	/// @brief Creates an identity matrix.
	/// @return The identity matrix.
	static Mat4 identity();
	/// @brief Creates a look-at view matrix.
	/// @param eye The camera position.
	/// @param center The target position.
	/// @param up The up vector.
	/// @return The view matrix.
	static Mat4 lookAt(const Vec3 &eye, const Vec3 &center, const Vec3 &up);
	/// @brief Creates a perspective projection matrix.
	/// @param fovy_rad The field of view in the Y direction, in radians.
	/// @param aspect The aspect ratio (width / height).
	/// @param near The near clipping plane.
	/// @param far The far clipping plane.
	/// @return The perspective projection matrix.
	static Mat4 perspective(
		float fovy_rad, float aspect, float near, float far
	);
	/// @brief Creates an orthographic projection matrix.
	/// @param left The left clipping plane.
	/// @param right The right clipping plane.
	/// @param bottom The bottom clipping plane.
	/// @param top The top clipping plane.
	/// @param near The near clipping plane.
	/// @param far The far clipping plane.
	/// @return The orthographic projection matrix.
	static Mat4 orthographic(
		float left, float right, float bottom, float top, float near, float far
	);
	/// @brief Creates a translation matrix.
	/// @param pos The translation vector.
	/// @return The translation matrix.
	static Mat4 translate(Vec3 pos);
	/// @brief Creates a rotation matrix around the X axis.
	/// @param radian The rotation angle in radians.
	/// @return The rotation matrix.
	static Mat4 rotationX(float radian);
	/// @brief Creates a rotation matrix around the Y axis.
	/// @param radian The rotation angle in radians.
	/// @return The rotation matrix.
	static Mat4 rotationY(float radian);
	/// @brief Creates a rotation matrix around the Z axis.
	/// @param radian The rotation angle in radians.
	/// @return The rotation matrix.
	static Mat4 rotationZ(float radian);
	/// @brief Creates a combined rotation matrix.
	/// @param radians The rotation angles (X, Y, Z) in radians.
	/// @return The rotation matrix.
	static Mat4 rotationXyz(Vec3 radians);
	/// @brief Creates a scaling matrix.
	/// @param v The scaling vector.
	/// @return The scaling matrix.
	static Mat4 scale(Vec3 v);
};

}  // namespace lili
