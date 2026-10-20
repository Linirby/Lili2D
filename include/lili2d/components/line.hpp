#pragma once

#include "lili2d/geometry/vec2.hpp"
#include <type_traits>

namespace lili {

/// @brief Geometry component representing a 2D line segment.
/// @note Starts at `TransformComponent::pos` and ends at `pos + dxy`. Uses
/// `Render2DComponent::color` for its line color.
struct LineComponent
{
    Vec2 dxy = { 0.0f, 0.0f }; ///< Offset vector (end_pos - start_pos) in pixels.
    float thickness = 1.0f;    ///< Line thickness in pixels.
};

static_assert(
    std::is_trivially_copyable_v<LineComponent>,
    "LineComponent must be trivially copyable"
);

} // namespace lili
