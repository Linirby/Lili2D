#pragma once

#include <type_traits>
namespace lili {

/// @brief Physics component that represent the Velocity
struct cVelocity
{
    float x = 0.0f; ///< X value
    float y = 0.0f; ///< Y value
};

static_assert(
    std::is_trivially_copyable_v<cVelocity>,
    "cVelocity component must be trivially copyable"
);

} // namespace lili
