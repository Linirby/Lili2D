#pragma once

#include "lili2d/geometry/vec2.hpp"
#include "lili2d/geometry/vec3.hpp"
#include "lili2d/render/scene/shapes/rect.hpp"
#include "lili2d/render/scene/shapes/circle.hpp"
#include "lili2d/render/scene/shapes/line.hpp"

namespace lili {

/// @brief Represents an Axis-Aligned Bounding Box.
struct AABB2 {
	/// @brief Default constructor.
	AABB2() = default;
	/// @brief Copy constructor.
	AABB2(const AABB2 &) = default;
	/// @brief Move constructor.
	AABB2(AABB2 &&) = default;
	/// @brief Construct AABB2 with two Vec2.
	/// @param pos The position of the topleft.
	/// @param the size of the bounding rect.
	AABB2(const Vec2 &pos, const Vec2 &size);
	/// @brief Construct AABB2 with a Rect
	/// @param rect The shape of the bounding rect
	AABB2(const Rect &rect);
	/// @brief Construct AABB2 with a Circle
	/// @param rect The shape of the bounding rect
	AABB2(const Circle &circle);
	/// @brief Construct AABB2 with a Line
	/// @param rect The shape of the bounding rect
	AABB2(const Line &line);
	/// @brief Move assignment operator.
	/// @return Reference to the assigned rectangle.
	AABB2& operator=(AABB2 &&) = default;

	Vec2 min;  ///< The minimum coordinates.
	Vec2 max;  ///< The maximum coordinates.

	/// @brief Checks if this AABB2 intersects with another.
	/// @param other The other AABB2 to test against.
	/// @return True if the AABB2s intersect, false otherwise.
	bool intersect(const AABB2 &other) const;
	/// @brief Checks if this AABB2 intersects with a rect.
	/// @param rect The rect to test against.
	/// @return True if there is an intersection, false otherwise.
	bool intersect(const Rect &rect) const;

	/// @brief Checks if this AABB2 contains another.
	/// @param other The other AABB2 to test against.
	/// @return True if this AABB2 contains the other, false otherwise.
	bool contains(const AABB2 &other) const;
	/// @brief Checks if this AABB2 contains a Rect.
	/// @param other The other Rect to test against.
	/// @return True if this AABB2 contains the Rect, false otherwise.
	bool contains(const Rect &rect) const;
	/// @brief Checks if this AABB2 contains a Circle.
	/// @param other The other Circle to test against.
	/// @return True if this AABB2 contains the Circle, false otherwise.
	bool contains(const Circle &circle) const;
	/// @brief Checks if this AABB2 contains a Line.
	/// @param other The other Line to test against.
	/// @return True if this AABB2 contains the Line, false otherwise.
	bool contains(const Line &line) const;

	/// @brief Draws a debug representation of this AABB.
	/// @param renderer The renderer.
	/// @param color The outline color.
	void debugDraw(Renderer *renderer, const Vec4 &color) const;
};

/// @brief Represents a 3D Axis-Aligned Bounding Box.
struct AABB3 {
	/// @brief Default constructor.
	AABB3() = default;
	/// @brief Copy constructor.
	AABB3(const AABB3 &) = default;
	/// @brief Move constructor.
	AABB3(AABB3 &&) = default;
	/// @brief Construct AABB3 with two Vec3.
	/// @param pos The position of the min corner.
	/// @param size The size of the bounding box.
	AABB3(const Vec3 &pos, const Vec3 &size);
	/// @brief Move assignment operator.
	/// @return Reference to the assigned box.
	AABB3& operator=(AABB3 &&) = default;

	Vec3 min;  ///< The minimum coordinates.
	Vec3 max;  ///< The maximum coordinates.

	/// @brief Checks if this AABB3 intersects with another.
	/// @param other The other AABB3 to test against.
	/// @return True if the AABB3s intersect, false otherwise.
	bool intersect(const AABB3 &other) const;

	/// @brief Checks if this AABB3 contains another.
	/// @param other The other AABB3 to test against.
	/// @return True if this AABB3 contains the other, false otherwise.
	bool contains(const AABB3 &other) const;
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
