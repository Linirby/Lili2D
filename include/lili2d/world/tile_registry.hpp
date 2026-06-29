#pragma once

#include "lili2d/world/tile.hpp"

namespace lili {

class TileRegistry {
public:
	TileRegistry(const TileRegistry&) = delete;
	TileRegistry &operator=(const TileRegistry &) = delete;

	static TileRegistry &get();

	uint16_t registerTile(const std::string &key, Tile &&tile);
	bool hasTile(const std::string &key) const;
	size_t tileCount() const;
	void clear();

	uint16_t getTileId(const std::string &key) const;
	const Tile &getTile(const std::string &key) const;
	const Tile &getTile(uint16_t tile_id) const;

private:
	std::unordered_map<std::string, uint16_t> key_to_id;
	std::vector<Tile> id_to_tile;

	TileRegistry();
};

}  // namespace lili
