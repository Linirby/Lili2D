#pragma once

#include "lili2d/render/scene/common/atlas_map.hpp"

namespace lili {

/// @brief Represents a single tile in the world.
struct Tile {
    SliceUV slice;          ///< Texture slice for rendering.
    bool is_solid = false;  ///< Flag indicating if tile is solid/collidable.

    /// @brief Default constructor.
    constexpr Tile() noexcept = default;

    /// @brief Constructs a Tile with a texture slice and collision flag.
    /// @param slice Texture UV slice.
    /// @param is_solid True if tile is solid/collidable.
    constexpr explicit Tile(
        const SliceUV& slice, bool is_solid = false
    ) noexcept
        : slice(slice), is_solid(is_solid) {}
};

}  // namespace lili
