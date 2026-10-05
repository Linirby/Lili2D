#pragma once

#include "lili2d/geometry/vec2.hpp"
#include <type_traits>

namespace lili {

/// @brief Component to represent transformation in the world.
struct TransformComponent
{
    lili::Vec2 pos = { 0.0f, 0.0f };   ///< Position X, Y in pixels.
    lili::Vec2 prev_pos = pos;         ///< Previous pos used for interpolation.
    lili::Vec2 scale = { 1.0f, 1.0f }; ///< Scale factor in X, Y.
    float rotation = 0.0f;             ///< Rotation in degrees.
};

static_assert(
    std::is_trivially_copyable_v<TransformComponent>,
    "TransformComponent must be trivially copyable"
);

} // namespace lili
