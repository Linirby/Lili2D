#pragma once

namespace lili {

/// @brief Represents a 3D mathematical vector.
struct Vec3 {
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	///< 3D Coordinates of the vector

	/// @brief Default constructor.
	Vec3() = default;
	/// @brief Constructs a 3D vector with given values.
	/// @param x The X component.
	/// @param y The Y component.
	/// @param z The Z component.
	Vec3(float x, float y, float z);

	/// @brief Subtraction operator.
	/// @param o The vector to subtract.
	/// @return The result of subtraction.
	Vec3 operator-(const Vec3 &o) const;
	/// @brief Subtraction assignment operator.
	/// @param o The vector to subtract.
	/// @return A reference to this vector.
	Vec3 operator-=(const Vec3 &o);
	/// @brief Addition operator.
	/// @param o The vector to add.
	/// @return The result of addition.
	Vec3 operator+(const Vec3 &o) const;
	/// @brief Addition assignment operator.
	/// @param o The vector to add.
	/// @return A reference to this vector.
	Vec3 operator+=(const Vec3 &o);
	/// @brief Scalar multiplication operator.
	/// @param scalar The scalar value.
	/// @return The scaled vector.
	Vec3 operator*(const float scalar) const;
	/// @brief Scalar multiplication assignment operator.
	/// @param scalar The scalar value.
	/// @return A reference to this vector.
	Vec3 operator*=(const float scalar);

	/// @brief Calculates the dot product with another vector.
	/// @param o The other vector.
	/// @return The dot product.
	float dot(const Vec3 &o) const;
	/// @brief Calculates the 3D cross product with another vector.
	/// @param o The other vector.
	/// @return The resulting cross product vector.
	Vec3 cross(const Vec3 &o) const;
	/// @brief Returns a normalized copy of this vector.
	/// @return The normalized vector.
	Vec3 normalized() const;
	/// @brief Linearly interpolates between this and another vector.
	/// @param o The target vector.
	/// @param t The interpolation factor.
	/// @return The interpolated vector.
	Vec3 lerp(const Vec3 &o, float t) const;
	/// @brief Gets the length (magnitude) of the vector.
	/// @return The length.
	float length() const;
};

}  // namespace lili
