#pragma once

#include "lili2d/render/scene/common/atlas_map.hpp"

namespace lili {

/// @brief Represents a single tile in the world.
struct Tile {
    /// @brief Default constructor.
    Tile() = default;
    /// @brief Constructs a Tile with a texture slice and collision flag.
    /// @param slice Texture UV slice.
    /// @param is_solid True if tile is solid/collidable.
    Tile(const SliceUV& slice, bool is_solid = false);

    SliceUV slice;          ///< Texture slice for rendering.
    bool is_solid = false;  ///< Flag indicating if tile is solid/collidable.
};

}  // namespace lili
