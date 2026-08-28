#include "lili2d/world/tilemap.hpp"

#include "lili2d/physics/aabb_collider.hpp"
#include "lili2d/render/renderer.hpp"
#include "lili2d/world/chunk.hpp"
#include "lili2d/world/tile_registry.hpp"

namespace lili {

TileMap::TileMap(lili::Vec2 tile_size) : tile_size(tile_size) {}

void
TileMap::setTile(std::string_view name, lili::Point3 pos) {
    TileRegistry& registry = TileRegistry::get();
    lili::Point3 chunk_pos = getChunkCoord(pos);
    lili::Point3 local_pos = getLocalCoord(pos);

    Chunk& chunk = chunks[chunk_pos];
    chunk.tiles[Chunk::flattenIndex(local_pos)] = registry.getTileID(name);
    chunk.dirty = true;
}

uint16_t
TileMap::getTile(lili::Point3 pos) const noexcept {
    lili::Point3 chunk_pos = getChunkCoord(pos);
    auto it = chunks.find(chunk_pos);
    if (it != chunks.end()) {
        lili::Point3 local_pos = getLocalCoord(pos);
        return it->second.tiles[Chunk::flattenIndex(local_pos)];
    }
    return 0;
}

bool
TileMap::checkCollision(lili::AABB3 target_aabb) const noexcept {
    TileRegistry& registry = TileRegistry::get();

    for (int z = target_aabb.min.z; z <= target_aabb.max.z; ++z)
        for (int y = target_aabb.min.y; y <= target_aabb.max.y; ++y)
            for (int x = target_aabb.min.x; x <= target_aabb.max.x; ++x) {
                uint16_t tile_id = getTile({x, y, z});
                if (tile_id == 0) continue;

                const Tile& tile = registry.getTile(tile_id);
                if (tile.is_solid) {
                    lili::AABB3 tile_aabb(
                        lili::Vec3(x, y, z), lili::Vec3(1.0f, 1.0f, 1.0f)
                    );
                    if (target_aabb.intersect(tile_aabb)) return true;
                }
            }
    return false;
}

void
TileMap::draw(Renderer* renderer, ThreadPool* thread_pool) {
    Camera* camera = renderer->getCamera();
    bool use_culling = (camera != nullptr);

    float chunk_sz_x = static_cast<float>(Chunk::SIZE) * tile_size.x;
    float chunk_sz_y = static_cast<float>(Chunk::SIZE) * tile_size.y;

    AABB2 bounds;
    if (use_culling) {
        Vec2 log_res = renderer->getLogicalResolution();
        bounds = camera->getViewportBounds(log_res.x, log_res.y);
        bounds.min.x -= chunk_sz_x;
        bounds.min.y -= chunk_sz_y;
        bounds.max.x += chunk_sz_x;
        bounds.max.y += chunk_sz_y;
    }

    int rebuilds_this_frame = 0;

    for (auto& pair : chunks) {
        Point3 chunk_pos = pair.first;
        const Chunk& chunk = pair.second;

        if (use_culling) {
            Vec2 chunk_pos_w(
                chunk_pos.x * chunk_sz_x, chunk_pos.y * chunk_sz_y
            );
            AABB2 chunk_aabb(chunk_pos_w, Vec2(chunk_sz_x, chunk_sz_y));

            if (!bounds.intersect(chunk_aabb)) continue;
        }

        if (chunk.dirty || chunk.rebuilding) {
            if (chunk.dirty) {
                if (rebuilds_this_frame >= 8) continue;
                rebuilds_this_frame++;
            }
            chunk.rebuildBatches(renderer, thread_pool, chunk_pos, tile_size);
        }
        for (auto& batch_pair : chunk.batches) batch_pair.second->draw();
    }
}

}  // namespace lili
