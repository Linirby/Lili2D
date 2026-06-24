#pragma once

namespace lili {

/// @brief Represents a 3D mathematical point.
struct Point3 {
	int x = 0; ///< X coordinate.
	int y = 0; ///< Y coordinate.
	int z = 0; ///< Z coordinate.

	/// @brief Default constructor.
	Point3() = default;
	/// @brief Constructs a 3D point with given values.
	/// @param x The X component.
	/// @param y The Y component.
	/// @param z The Z component.
	Point3(int x, int y, int z);

	/// @brief Subtraction operator.
	/// @param o The point to subtract.
	/// @return The result of subtraction.
	Point3 operator-(const Point3 &o) const;
	/// @brief Subtraction assignment operator.
	/// @param o The point to subtract.
	/// @return A reference to this point.
	Point3 operator-=(const Point3 &o);
	/// @brief Addition operator.
	/// @param o The point to add.
	/// @return The result of addition.
	Point3 operator+(const Point3 &o) const;
	/// @brief Addition assignment operator.
	/// @param o The point to add.
	/// @return A reference to this point.
	Point3 operator+=(const Point3 &o);
	/// @brief Scalar multiplication operator.
	/// @param scalar The scalar value.
	/// @return The scaled point.
	Point3 operator*(const int scalar) const;
	/// @brief Scalar multiplication assignment operator.
	/// @param scalar The scalar value.
	/// @return A reference to this point.
	Point3 operator*=(const float scalar);
};

}  // namespace lili
