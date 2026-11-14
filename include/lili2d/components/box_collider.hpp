#pragma once

#include "lili2d/geometry/vec2.hpp"
#include <type_traits>
namespace lili {

/// @brief Physics component that represents a Box collider
struct cBoxCollider
{
    Vec2 offset;          ///< Offset from the entity position (Transform)
    Vec2 size;            ///< Size of the box
    bool physical = true; ///< True stops entities, False acts as a trigger zone
};

static_assert(
    std::is_trivially_copyable_v<cBoxCollider>,
    "cBoxCollider component must be trivially copyable"
);

} // namespace lili
