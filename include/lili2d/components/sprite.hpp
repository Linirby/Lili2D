#pragma once

#include "lili2d/render/common/slice_uv.hpp"
#include <type_traits>

namespace lili {

/// @brief Quad component representing a 2D textured sprite.
/// @note Uses `cRender2D::size` for its quad dimensions and
/// `cRender2D::color` for its tint.
struct cSprite
{
    SliceUV slice{}; ///< The UV slice and texture pointer.
};

static_assert(
    std::is_trivially_copyable_v<cSprite>,
    "cSprite must be trivially copyable"
);

} // namespace lili
