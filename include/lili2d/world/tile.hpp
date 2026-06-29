#pragma once

#include "lili2d/render/scene/common/atlas_map.hpp"

namespace lili {

/// @brief Represents a single tile in the world.
struct Tile {
	Tile() = default;
	Tile(
		const SliceUV &slice,
		bool is_solid = false
	);

	SliceUV slice;
	bool is_solid = false;
};

}  // namespace lili
