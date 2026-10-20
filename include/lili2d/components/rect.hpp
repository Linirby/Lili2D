#pragma once

#include <type_traits>

namespace lili {

/// @brief Quad component representing a 2D rectangle.
/// @note Uses `Render2DComponent::size` for its width/height and
/// `Render2DComponent::color` for its fill/outline color.
struct RectComponent
{
    float thickness = 1.0f; ///< Outline thickness in pixels when hollow.
    bool hollow = false;    ///< True for outline only, false for filled.
};

static_assert(
    std::is_trivially_copyable_v<RectComponent>,
    "RectComponent must be trivially copyable"
);

} // namespace lili
