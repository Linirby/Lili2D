#pragma once

#include <type_traits>
namespace lili {

/// @brief Physics component that reprensent the gravity force
struct cGravity
{
    float force = 9.81f;           ///< The force
    float scale = 1.0f;            ///< Multiplier for the force
    float max_fall_speed = 500.0f; ///< Limit that cap fall speed
};

static_assert(
    std::is_trivially_copyable_v<cGravity>,
    "cGravity component must be trivially copyable"
);

} // namespace lili
