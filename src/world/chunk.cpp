#include "lili2d/world/chunk.hpp"
#include "lili2d/world/tile_registry.hpp"

namespace lili {

Chunk::Chunk() {
	tiles.resize(SIZE * SIZE * SIZE, 0);
}

size_t Chunk::flattenIndex(lili::Point3 local_pos) {
	return (
		local_pos.x +
		local_pos.y * SIZE +
		local_pos.z * SIZE * SIZE
	);
}

void Chunk::rebuildBatches(
	Renderer *renderer, Point3 chunk_pos, const Vec2 &tile_size
) const {
	for (auto &pair : batches) {
		pair.second->begin();
	}
	TileRegistry &registry = TileRegistry::get();

	for (int z = 0; z < SIZE; ++z) {
		for (int y = 0; y < SIZE; ++y) {
			for (int x = 0; x < SIZE; ++x) {
				uint16_t tile_id = tiles[flattenIndex({x, y, z})];
				if (tile_id == 0) continue;
				const Tile &tile = registry.getTile(tile_id);
				if (!tile.slice.texture) continue;

				BatchKey key{tile.slice.texture, z};
				if (!batches.contains(key)) {
					batches[key] = std::make_unique<SpriteBatch>(
						renderer, tile.slice.texture
					);
					batches[key]->setLayer(static_cast<float>(z));
				}
			}
		}
	}

	for (int z = 0; z < SIZE; ++z) {
		for (int y = 0; y < SIZE; ++y) {
			for (int x = 0; x < SIZE; ++x) {
				uint16_t tile_id = tiles[flattenIndex({x, y, z})];
				if (tile_id == 0) continue;
				const Tile &tile = registry.getTile(tile_id);
				if (!tile.slice.texture) continue;

				BatchKey key = BatchKey(tile.slice.texture, z);
				int world_x = chunk_pos.x * SIZE + x;
				int world_y = chunk_pos.y * SIZE + y;
				Vec2 pos(
					static_cast<float>(world_x) * tile_size.x,
					static_cast<float>(world_y) * tile_size.y
				);
				batches[key]->draw(tile.slice, pos);
			}
		}
	}

	for (auto &pair : batches) {
		pair.second->end();
	}

	dirty = false;
}

}  // namespace lili
