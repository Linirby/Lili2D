#include "lili2d/world/tile.hpp"

namespace lili {

Tile::Tile(
	const SliceUV &slice,
	bool is_solid
) : slice(slice), is_solid(is_solid) {
}

}  // namespace lili
