#pragma once

namespace lili {

/// @brief Represents a 2D mathematical vector.
struct Vec2 {
	float x = 0.0f; ///< X coordinate.
	float y = 0.0f; ///< Y coordinate.

	/// @brief Default constructor.
	Vec2() = default;
	/// @brief Constructs a 2D vector with given values.
	/// @param x The X component.
	/// @param y The Y component.
	Vec2(float x, float y);

	/// @brief Subtraction operator.
	/// @param o The vector to subtract.
	/// @return The result of subtraction.
	Vec2 operator-(const Vec2 &o) const;
	/// @brief Subtraction assignment operator.
	/// @param o The vector to subtract.
	/// @return A reference to this vector.
	Vec2 operator-=(const Vec2 &o);
	/// @brief Addition operator.
	/// @param o The vector to add.
	/// @return The result of addition.
	Vec2 operator+(const Vec2 &o) const;
	/// @brief Addition assignment operator.
	/// @param o The vector to add.
	/// @return A reference to this vector.
	Vec2 operator+=(const Vec2 &o);
	/// @brief Scalar multiplication operator.
	/// @param scalar The scalar value.
	/// @return The scaled vector.
	Vec2 operator*(const float scalar) const;
	/// @brief Scalar multiplication assignment operator.
	/// @param scalar The scalar value.
	/// @return A reference to this vector.
	Vec2 operator*=(const float scalar);
	/// @brief Is equal assignment operator.
	/// @param o The vector to check with.
	// return A true if they're equal otherwise false.
	bool operator==(const Vec2 &o) const;

	/// @brief Calculates the dot product with another vector.
	/// @param o The other vector.
	/// @return The dot product.
	float dot(const Vec2 &o) const;
	/// @brief Calculates the 2D cross product with another vector.
	/// @param o The other vector.
	/// @return The cross product scalar value.
	float cross(const Vec2 &o) const;
	/// @brief Returns a normalized copy of this vector.
	/// @return The normalized vector.
	Vec2 normalized() const;
	/// @brief Linearly interpolates between this and another vector.
	/// @param o The target vector.
	/// @return The interpolated vector.
	Vec2 lerp(const Vec2 &o, float t) const;
	/// @brief Gets the length (magnitude) of the vector.
	/// @return The length.
	float length() const;
};

}  // namespace lili
