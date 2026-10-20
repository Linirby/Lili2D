#pragma once

#include "lili2d/render/common/atlas_map.hpp"
#include <type_traits>

namespace lili {

/// @brief Quad component representing a 2D textured sprite.
/// @note Uses `Render2DComponent::size` for its quad dimensions and
/// `Render2DComponent::color` for its tint.
struct SpriteComponent
{
    SliceUV slice{}; ///< The UV slice and texture pointer.
};

static_assert(
    std::is_trivially_copyable_v<SpriteComponent>,
    "SpriteComponent must be trivially copyable"
);

} // namespace lili
