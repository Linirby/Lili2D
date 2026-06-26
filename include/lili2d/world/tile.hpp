#pragma once

#include "lili2d/render/scene/common/atlas_map.hpp"

namespace lili {

struct Tile {
	Tile() = default;
	Tile(
		const SliceUV &slice,
		bool is_solid = false
	);

	SliceUV slice;
	bool is_solid = false;
};

inline Tile::Tile(
	const SliceUV &slice,
	bool is_solid
) : slice(slice), is_solid(is_solid) {
}

}  // namespace lili
