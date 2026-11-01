#pragma once

#include "lili2d/render/gpu/texture.hpp"

namespace lili {

/// @brief Represents a slice of a texture with UV coordinates.
struct SliceUV
{
    Texture* texture = nullptr; ///< The texture.
    float u_min = 0.0f;         ///< Minimum U coordinate.
    float v_min = 0.0f;         ///< Minimum V coordinate.
    float u_max = 1.0f;         ///< Maximum U coordinate.
    float v_max = 1.0f;         ///< Maximum V coordinate.
    float width = 0.0f;         ///< Frame width.
    float height = 0.0f;        ///< Frame height.
};

} // namespace lili
