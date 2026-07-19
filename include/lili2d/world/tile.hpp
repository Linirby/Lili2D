#pragma once

#include "lili2d/render/scene/common/atlas_map.hpp"

namespace lili {

/// @brief Represents a single tile in the world.
struct Tile {
	/// @brief Default constructor.
	Tile() = default;
	/// @brief Constructs a tile.
	/// @param slice The texture slice UV.
	/// @param is_solid Whether the tile is solid for physical collisions.
	Tile(
		const SliceUV &slice,
		bool is_solid = false
	);

	/// @brief The UV texture coordinates for the tile.
	SliceUV slice;
	/// @brief Flag indicating if this tile is physically solid.
	bool is_solid = false;
};

}  // namespace lili
