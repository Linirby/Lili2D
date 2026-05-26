#pragma once

namespace lili {

/// \brief Represents an integer point in 3D space.
struct Point3 {
	int x, y, z;

	/**
	 * \brief Assignment operator.
	 * \param o The point to copy from.
	 * \return A reference to this point.
	 */
	Point3 operator=(const Point3 &o);
};

}  // namespace lili
