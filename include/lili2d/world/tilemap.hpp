#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>

#include "lili2d/geometry/vec2.hpp"
#include "lili2d/geometry/point3.hpp"
#include "lili2d/physics/collision.hpp"
#include "lili2d/render/renderer.hpp"
#include "lili2d/world/chunk.hpp"

namespace lili {

struct Point3Hash {
	std::size_t operator()(const lili::Point3& k) const;
};

struct Point3Equal {
	bool operator()(const lili::Point3& lhs, const lili::Point3& rhs) const;
};

class TileMap {
public:
	TileMap(const lili::Vec2 &tile_size_px);

	void setTile(const std::string &name, lili::Point3 pos);
	uint16_t getTile(lili::Point3 pos) const;

	bool checkCollision(const lili::AABB3 &target_aabb) const;
	void draw(Renderer *renderer);

private:
	lili::Vec2 tile_size;
	std::unordered_map<lili::Point3, Chunk, Point3Hash, Point3Equal> chunks;

	static lili::Point3 getChunkCoord(lili::Point3 pos);
	static lili::Point3 getLocalCoord(lili::Point3 pos);
};

}  // namespace lili
