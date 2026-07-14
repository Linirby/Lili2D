#include "lili2d/world/tilemap.hpp"
#include "lili2d/world/tile_registry.hpp"
#include "lili2d/render/renderer.hpp"
#include "lili2d/physics/collision.hpp"
#include "lili2d/world/chunk.hpp"

namespace lili {

bool Point3Compare::operator()(
	const lili::Point3& lhs, const lili::Point3& rhs
) const {
	if (lhs.z != rhs.z)
		return lhs.z < rhs.z;
	if (lhs.y != rhs.y)
		return lhs.y < rhs.y;
	return lhs.x < rhs.x;
}

TileMap::TileMap(const lili::Vec2 &tile_size) : tile_size(tile_size) {}

void TileMap::setTile(const std::string &name, lili::Point3 pos) {
	TileRegistry &registry = TileRegistry::get();
	lili::Point3 chunk_pos = getChunkCoord(pos);
	lili::Point3 local_pos = getLocalCoord(pos);

	Chunk &chunk = chunks[chunk_pos];
	chunk.tiles[Chunk::flattenIndex(local_pos)] = registry.getTileID(name);
	chunk.dirty = true;
}

uint16_t TileMap::getTile(lili::Point3 pos) const {
	lili::Point3 chunk_pos = getChunkCoord(pos);
	auto it = chunks.find(chunk_pos);
	if (it != chunks.end()) {
		lili::Point3 local_pos = getLocalCoord(pos);
		return it->second.tiles[Chunk::flattenIndex(local_pos)];
	}
	return 0;
}

bool TileMap::checkCollision(const lili::AABB3 &target_aabb) const {
	TileRegistry &registry = TileRegistry::get();

	for (int z = target_aabb.min.z; z <= target_aabb.max.z; ++z) {
		for (int y = target_aabb.min.y; y <= target_aabb.max.y; ++y) {
			for (int x = target_aabb.min.x; x <= target_aabb.max.x; ++x) {
				uint16_t tile_id = getTile({x, y, z});
				if (tile_id == 0) continue;

				const Tile &tile = registry.getTile(tile_id);
				if (tile.is_solid) {
					lili::AABB3 tile_aabb{
						lili::Vec3{
							static_cast<float>(x),
							static_cast<float>(y),
							static_cast<float>(z)
						},
						lili::Vec3{1.0f, 1.0f, 1.0f}
					};
					if (target_aabb.intersect(tile_aabb))
						return true;
				}
			}
		}
	}
	return false;
}

void TileMap::draw(Renderer *renderer, ThreadPool *thread_pool) {
	Camera *camera = renderer->getCamera();
	bool use_culling = (camera != nullptr);

	float chunk_sz_x = static_cast<float>(Chunk::SIZE) * tile_size.x;
	float chunk_sz_y = static_cast<float>(Chunk::SIZE) * tile_size.y;

	AABB2 bounds;
	if (use_culling) {
		bounds = camera->getViewportBounds(
			static_cast<float>(renderer->getSwapchainWidth()),
			static_cast<float>(renderer->getSwapchainHeight())
		);
		bounds.min.x -= chunk_sz_x;
		bounds.min.y -= chunk_sz_y;
		bounds.max.x += chunk_sz_x;
		bounds.max.y += chunk_sz_y;
	}

	for (auto &pair : chunks) {
		const Point3 &chunk_pos = pair.first;
		const Chunk &chunk = pair.second;

		if (use_culling) {
			Vec2 chunk_pos_w{
				static_cast<float>(chunk_pos.x) * chunk_sz_x,
				static_cast<float>(chunk_pos.y) * chunk_sz_y
			};
			AABB2 chunk_aabb{chunk_pos_w, Vec2{chunk_sz_x, chunk_sz_y}};

			if (!bounds.intersect(chunk_aabb)) {
				continue;
			}
		}

		if (chunk.dirty || chunk.rebuilding)
			chunk.rebuildBatches(renderer, thread_pool, chunk_pos, tile_size);
		for (auto &batch_pair : chunk.batches)
			batch_pair.second->draw();
	}
}

lili::Point3 TileMap::getChunkCoord(lili::Point3 pos) {
	auto floor_div = [](int a, int b) {
		int res = a / b;
		int rem = a % b;
		if (rem != 0 && ((a < 0) ^ (b < 0)))
			res--;
		return res;
	};
	return {
		floor_div(pos.x, Chunk::SIZE),
		floor_div(pos.y, Chunk::SIZE),
		floor_div(pos.z, Chunk::SIZE)
	};
}

lili::Point3 TileMap::getLocalCoord(lili::Point3 pos) {
	auto floor_mod = [](int a, int b) {
		int res = a % b;
		if (res != 0 && ((a < 0) ^ (b < 0)))
			res += b;
		return res;
	};
	return {
		floor_mod(pos.x, Chunk::SIZE),
		floor_mod(pos.y, Chunk::SIZE),
		floor_mod(pos.z, Chunk::SIZE)
	};
}

}  // namespace lili
