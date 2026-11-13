#pragma once

#include <type_traits>
namespace lili {

/// @brief Physics component that reprensent the friction on a surface
struct cFriction
{
    float amount = 400.0f; ///< Bigger amount is more friction
};

static_assert(
    std::is_trivially_copyable_v<cFriction>,
    "cFriction component must be trivially copyable"
);

} // namespace lili
