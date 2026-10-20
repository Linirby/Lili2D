#pragma once

#include <cstdint>
#include <type_traits>

namespace lili {

/// @brief Geometry component representing a 2D circle.
/// @note Uses `Render2DComponent::color` for its fill/outline color.
struct CircleComponent
{
    float radius = 1.0f;    ///< Circle radius in pixels.
    uint32_t segments = 24; ///< Number of polygon segments.
    float thickness = 1.0f; ///< Outline thickness in pixels when hollow.
    bool hollow = false;    ///< True for outline only, false for filled.
};

static_assert(
    std::is_trivially_copyable_v<CircleComponent>,
    "CircleComponent must be trivially copyable"
);

} // namespace lili
