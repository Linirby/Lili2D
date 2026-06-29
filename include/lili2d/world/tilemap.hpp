#pragma once

#include <cstdint>
#include <string>
#include <map>

#include "lili2d/geometry/vec2.hpp"
#include "lili2d/geometry/point3.hpp"
#include "lili2d/physics/collision.hpp"
#include "lili2d/render/renderer.hpp"
#include "lili2d/world/chunk.hpp"

namespace lili {

/// @brief Comparison functor for Point3, used in std::map.
struct Point3Compare {
	/// @brief Compares two Point3 objects.
	/// @param lhs The left-hand side point.
	/// @param rhs The right-hand side point.
	/// @return True if lhs < rhs.
	bool operator()(const lili::Point3& lhs, const lili::Point3& rhs) const;
};

/// @brief Manages a grid-based world of chunks and tiles.
class TileMap {
public:
	/// @brief Constructs a tilemap.
	/// @param tile_size_px The size of each tile in pixels.
	TileMap(const lili::Vec2 &tile_size_px);

	/// @brief Sets a tile at a specific world position.
	/// @param name The registry name of the tile.
	/// @param pos The 3D grid position.
	void setTile(const std::string &name, lili::Point3 pos);
	/// @brief Gets the tile ID at a specific world position.
	/// @param pos The 3D grid position.
	/// @return The tile ID, or 0 if empty.
	uint16_t getTile(lili::Point3 pos) const;

	/// @brief Checks for collision against the solid tiles in the map.
	/// @param target_aabb The bounding box to check.
	/// @return True if there is a collision, false otherwise.
	bool checkCollision(const lili::AABB3 &target_aabb) const;
	/// @brief Submits the visible chunks for drawing.
	/// @param renderer The renderer.
	void draw(Renderer *renderer);

private:
	lili::Vec2 tile_size;
	std::map<lili::Point3, Chunk, Point3Compare> chunks;

	/// @brief Gets the chunk coordinate for a given world position.
	/// @param pos The world position.
	/// @return The chunk coordinate.
	static lili::Point3 getChunkCoord(lili::Point3 pos);
	/// @brief Gets the local coordinate within a chunk for a world position.
	/// @param pos The world position.
	/// @return The local coordinate.
	static lili::Point3 getLocalCoord(lili::Point3 pos);
};

}  // namespace lili
