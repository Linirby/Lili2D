#pragma once

namespace lili {

/// @brief Represents a 2D mathematical point.
struct Point2 {
	int x = 0; ///< X coordinate.
	int y = 0; ///< Y coordinate.

	/// @brief Default constructor.
	Point2() = default;
	/// @brief Constructs a 2D point with given values.
	/// @param x The X component.
	/// @param y The Y component.
	Point2(int x, int y);

	/// @brief Subtraction operator.
	/// @param o The point to subtract.
	/// @return The result of subtraction.
	Point2 operator-(const Point2 &o) const;
	/// @brief Subtraction assignment operator.
	/// @param o The point to subtract.
	/// @return A reference to this point.
	Point2 operator-=(const Point2 &o);
	/// @brief Addition operator.
	/// @param o The point to add.
	/// @return The result of addition.
	Point2 operator+(const Point2 &o) const;
	/// @brief Addition assignment operator.
	/// @param o The point to add.
	/// @return A reference to this point.
	Point2 operator+=(const Point2 &o);
	/// @brief Scalar multiplication operator.
	/// @param scalar The scalar value.
	/// @return The scaled point.
	Point2 operator*(const int scalar) const;
	/// @brief Scalar multiplication assignment operator.
	/// @param scalar The scalar value.
	/// @return A reference to this point.
	Point2 operator*=(const float scalar);
};

}  // namespace lili
