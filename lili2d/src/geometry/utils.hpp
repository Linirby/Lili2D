#pragma once

namespace lili {

/**
 * \brief Converts degrees to radians.
 * \param degrees The angle in degrees.
 * \return The angle in radians.
 */
inline float deg_to_rad(float degrees) {
	return degrees * 3.14159265359f / 180.0f;
}

}  // namespace lili
