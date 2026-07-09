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

ChunkMeshData Chunk::generateMeshData(
	Point3 chunk_pos, const Vec2 &tile_size
) const {
	ChunkMeshData chunk_mesh;
	TileRegistry &registry = TileRegistry::get();
	std::unordered_map<BatchKey, MeshData, BatchKeyHash> temp_meshes;

	for (int z = 0; z < SIZE; ++z) {
		for (int y = 0; y < SIZE; ++y) {
			for (int x = 0; x < SIZE; ++x) {
				uint16_t tile_id = tiles[flattenIndex({x, y, z})];
				if (tile_id == 0) continue;
				const Tile &tile = registry.getTile(tile_id);
				if (!tile.slice.texture) continue;

				BatchKey key{tile.slice.texture, z};
				int world_x = chunk_pos.x * SIZE + x;
				int world_y = chunk_pos.y * SIZE + y;
				Vec2 pos(
					static_cast<float>(world_x) * tile_size.x,
					static_cast<float>(world_y) * tile_size.y
				);

				SpriteBatch::appendSpriteToMesh(
					temp_meshes[key], tile.slice, pos
				);
			}
		}
	}

	chunk_mesh.batches.reserve(temp_meshes.size());
	for (auto &pair : temp_meshes) {
		chunk_mesh.batches.push_back({pair.first, std::move(pair.second)});
	}

	return chunk_mesh;
}

void Chunk::uploadMeshData(
	Renderer *renderer, ChunkMeshData &&mesh_data
) const {
	for (auto &pair : batches) {
		pair.second->clear();
	}

	for (auto &batch_data : mesh_data.batches) {
		const BatchKey &key = batch_data.key;
		if (!batches.contains(key)) {
			batches[key] = std::make_unique<SpriteBatch>(
				renderer, key.texture
			);
			batches[key]->setLayer(static_cast<float>(key.z));
		}
		batches[key]->setMeshData(std::move(batch_data.mesh_data));
	}
}

void Chunk::rebuildBatches(
	Renderer *renderer, Point3 chunk_pos, const Vec2 &tile_size
) const {
	ChunkMeshData mesh_data = generateMeshData(chunk_pos, tile_size);
	uploadMeshData(renderer, std::move(mesh_data));
	dirty = false;
}

}  // namespace lili
