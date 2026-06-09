#pragma once

namespace lili {

/// @brief Represents a 4D mathematical vector.
struct Vec4 {
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	float w = 0.0f;
	///< 4D Coordinates of the vector

	/// @brief Default constructor.
	Vec4() = default;
	/// @brief Constructs a 4D vector with given values.
	/// @param x The X component.
	/// @param y The Y component.
	/// @param z The Z component.
	/// @param w The W component.
	Vec4(float x, float y, float z, float w);

	/// @brief Subtraction operator.
	/// @param o The vector to subtract.
	/// @return The result of subtraction.
	Vec4 operator-(const Vec4 &o) const;
	/// @brief Subtraction assignment operator.
	/// @param o The vector to subtract.
	/// @return A reference to this vector.
	Vec4 operator-=(const Vec4 &o);
	/// @brief Addition operator.
	/// @param o The vector to add.
	/// @return The result of addition.
	Vec4 operator+(const Vec4 &o) const;
	/// @brief Addition assignment operator.
	/// @param o The vector to add.
	/// @return A reference to this vector.
	Vec4 operator+=(const Vec4 &o);
	/// @brief Scalar multiplication operator.
	/// @param scalar The scalar value.
	/// @return The scaled vector.
	Vec4 operator*(const float scalar) const;
	/// @brief Scalar multiplication assignment operator.
	/// @param scalar The scalar value.
	/// @return A reference to this vector.
	Vec4 operator*=(const float scalar);

	/// @brief Calculates the dot product with another vector.
	/// @param o The other vector.
	/// @return The dot product.
	float dot(const Vec4 &o) const;
	/// @brief Calculates the cross product with another vector.
	/// @param o The other vector.
	/// @return The resulting cross product vector.
	Vec4 cross(const Vec4 &o) const;
	/// @brief Returns a normalized copy of this vector.
	/// @return The normalized vector.
	Vec4 normalized() const;
	/// @brief Gets the length (magnitude) of the vector.
	/// @return The length.
	float length() const;
};

}  // namespace lili
