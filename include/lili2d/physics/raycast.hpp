#pragma once

namespace lili {

/// @brief Contains the result of a raycast operation.
struct RaycastResult {
    bool hit = false;  ///< True if the raycast hit something.
    int hit_x = 0;     ///< Hit X coordinate.
    int hit_y = 0;     ///< Hit Y coordinate.
    int hit_z = 0;     ///< Hit Z coordinate.
    int adj_x = 0;     ///< Adjacent X coordinate (surface normal).
    int adj_y = 0;     ///< Adjacent Y coordinate (surface normal).
    int adj_z = 0;     ///< Adjacent Z coordinate (surface normal).
};

}  // namespace lili
