#pragma once

namespace lili {

/// @brief Converts degrees to radians.
/// @param degrees The angle in degrees.
/// @return The angle in radians.
[[nodiscard]] constexpr inline float
degToRad(float degrees) noexcept {
    return degrees * 3.14159265358979323846f / 180.0f;
}

/// @brief Converts radians to degrees.
/// @param radians The angle in radians.
/// @return The angle in degrees.
[[nodiscard]] constexpr inline float
radToDeg(float radians) noexcept {
    return radians * 180.0f / 3.14159265358979323846f;
}

}  // namespace lili
