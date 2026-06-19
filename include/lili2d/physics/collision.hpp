#pragma once

#include "lili2d/geometry/vec2.hpp"
#include "lili2d/render/scene/shapes/rect.hpp"
#include "lili2d/render/scene/shapes/circle.hpp"
#include "lili2d/render/scene/shapes/line.hpp"

namespace lili {

/// @brief Represents an Axis-Aligned Bounding Box.
struct AABB {
	/// @brief Default constructor.
	AABB() = default;
	/// @brief Copy constructor.
	AABB(const AABB &) = default;
	/// @brief Move constructor.
	AABB(AABB &&) = default;
	/// @brief Construct AABB with two Vec2.
	/// @param pos The position of the topleft.
	/// @param the size of the bounding rect.
	AABB(const Vec2 &pos, const Vec2 &size);
	/// @brief Construct AABB with a Rect
	/// @param rect The shape of the bounding rect
	AABB(const Rect &rect);
	/// @brief Construct AABB with a Circle
	/// @param rect The shape of the bounding rect
	AABB(const Circle &circle);
	/// @brief Construct AABB with a Line
	/// @param rect The shape of the bounding rect
	AABB(const Line &line);
	/// @brief Move assignment operator.
	/// @return Reference to the assigned rectangle.
	AABB& operator=(AABB &&) = default;

	Vec2 min;  ///< The minimum coordinates.
	Vec2 max;  ///< The maximum coordinates.

	/// @brief Checks if this AABB intersects with another.
	/// @param other The other AABB to test against.
	/// @return True if the AABBs intersect, false otherwise.
	bool intersect(const AABB &other) const;
	/// @brief Checks if this AABB intersects with a rect.
	/// @param rect The rect to test against.
	/// @return True if there is an intersection, false otherwise.
	bool intersect(const Rect &rect) const;

	/// @brief Checks if this AABB contains another.
	/// @param other The other AABB to test against.
	/// @return True if this AABB contains the other, false otherwise.
	bool contains(const AABB &other) const;
	/// @brief Checks if this AABB contains a Rect.
	/// @param other The other Rect to test against.
	/// @return True if this AABB contains the Rect, false otherwise.
	bool contains(const Rect &rect) const;
	/// @brief Checks if this AABB contains a Circle.
	/// @param other The other Circle to test against.
	/// @return True if this AABB contains the Circle, false otherwise.
	bool contains(const Circle &circle) const;
	/// @brief Checks if this AABB contains a Line.
	/// @param other The other Line to test against.
	/// @return True if this AABB contains the Line, false otherwise.
	bool contains(const Line &line) const;

	void debugDraw(Renderer *renderer, const Vec4 &color) const;
};

/// @brief Contains the result of a raycast operation.
struct RaycastResult {
	bool hit = false;  ///< True if the raycast hit something.
	int hit_x = 0; ///< Hit X coordinate.
	int hit_y = 0; ///< Hit Y coordinate.
	int hit_z = 0; ///< Hit Z coordinate.
	int adj_x = 0; ///< Adjacent X coordinate (surface normal).
	int adj_y = 0; ///< Adjacent Y coordinate (surface normal).
	int adj_z = 0; ///< Adjacent Z coordinate (surface normal).
};

}  // namespace lili
