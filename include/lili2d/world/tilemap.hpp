#pragma once

#include <cstdint>
#include <map>
#include <string_view>

#include "lili2d/geometry/point3.hpp"
#include "lili2d/geometry/vec2.hpp"
#include "lili2d/physics/aabb_collider.hpp"
#include "lili2d/render/renderer.hpp"
#include "lili2d/world/chunk.hpp"

namespace lili {

class ThreadPool;

/// @brief Comparison functor for Point3, used in std::map.
struct Point3Compare {
    /// @brief Compares two Point3 objects.
    /// @param lhs The left-hand side point.
    /// @param rhs The right-hand side point.
    /// @return True if lhs < rhs.
    [[nodiscard]] constexpr bool
    operator()(lili::Point3 lhs, lili::Point3 rhs) const noexcept {
        if (lhs.z != rhs.z) return lhs.z < rhs.z;
        if (lhs.y != rhs.y) return lhs.y < rhs.y;
        return lhs.x < rhs.x;
    }
};

/// @brief Manages a grid-based world of chunks and tiles.
class TileMap {
public:
    /// @brief Constructs a tilemap.
    /// @param tile_size_px The size of each tile in pixels.
    explicit TileMap(lili::Vec2 tile_size_px);

    /// @brief Move constructor.
    TileMap(TileMap&&) noexcept = default;
    /// @brief Move assignment operator.
    TileMap&
    operator=(TileMap&&) noexcept = default;

    /// @brief Deleted copy constructor.
    TileMap(const TileMap&) = delete;
    /// @brief Deleted copy assignment operator.
    TileMap&
    operator=(const TileMap&) = delete;

    /// @brief Sets a tile at a specific world position.
    /// @param name The registry name of the tile.
    /// @param pos The 3D grid position.
    void
    setTile(std::string_view name, lili::Point3 pos);
    /// @brief Gets the tile ID at a specific world position.
    /// @param pos The 3D grid position.
    /// @return The tile ID, or 0 if empty.
    [[nodiscard]] uint16_t
    getTile(lili::Point3 pos) const noexcept;

    /// @brief Checks for collision against the solid tiles in the map.
    /// @param target_aabb The bounding box to check.
    /// @return True if there is a collision, false otherwise.
    [[nodiscard]] bool
    checkCollision(lili::AABB3 target_aabb) const noexcept;
    /// @brief Submits the visible chunks for drawing.
    /// @param renderer The renderer.
    /// @param thread_pool Pointer to the ThreadPool.
    void
    draw(Renderer* renderer, ThreadPool* thread_pool = nullptr);

    /// @brief Gets the size of each tile in pixels.
    /// @return The tile size.
    [[nodiscard]] inline lili::Vec2
    getTileSize() const noexcept {
        return tile_size;
    }

    /// @brief Computes chunk coordinate containing a world grid position.
    /// @param pos The 3D grid position.
    /// @return The chunk coordinate.
    [[nodiscard]] static constexpr inline lili::Point3
    getChunkCoord(lili::Point3 pos) noexcept {
        auto floor_div = [](int a, int b) {
            int res = a / b;
            int rem = a % b;
            if (rem != 0 && ((a < 0) ^ (b < 0))) res--;
            return res;
        };
        return {
            floor_div(pos.x, Chunk::SIZE), floor_div(pos.y, Chunk::SIZE),
            floor_div(pos.z, Chunk::SIZE)
        };
    }

    /// @brief Computes local coordinate within chunk from a world grid
    /// position.
    /// @param pos The 3D grid position.
    /// @return The local chunk coordinate.
    [[nodiscard]] static constexpr inline lili::Point3
    getLocalCoord(lili::Point3 pos) noexcept {
        auto floor_mod = [](int a, int b) {
            int res = a % b;
            if (res != 0 && ((a < 0) ^ (b < 0))) res += b;
            return res;
        };
        return {
            floor_mod(pos.x, Chunk::SIZE), floor_mod(pos.y, Chunk::SIZE),
            floor_mod(pos.z, Chunk::SIZE)
        };
    }

private:
    /// @brief Dimensions of a single tile in pixels.
    lili::Vec2 tile_size;
    /// @brief Map of 3D chunk coordinates to Chunk instances.
    std::map<lili::Point3, Chunk, Point3Compare> chunks;
};

}  // namespace lili
