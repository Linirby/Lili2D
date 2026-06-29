#include "lili2d/world/tile_registry.hpp"

namespace lili {

TileRegistry &TileRegistry::get() {
	static TileRegistry instance;
	return instance;
}

uint16_t TileRegistry::registerTile(const std::string &key, Tile &&tile) {
	auto it = key_to_id.find(key);
	if (it != key_to_id.end()) {
		id_to_tile[it->second].~Tile();
		new (&id_to_tile[it->second]) Tile(std::move(tile));
		return it->second;
	}

	if (id_to_tile.size() >= UINT16_MAX)
		throw std::runtime_error(
			"TileRegistry has reach the max capacity for uint16_t."
		);
	
	uint16_t new_id = id_to_tile.size();
	id_to_tile.push_back(std::move(tile));
	key_to_id[key] = new_id;
	return new_id;
}

bool TileRegistry::hasTile(const std::string &key) const {
	return key_to_id.contains(key);
}

size_t TileRegistry::tileCount() const {
	return id_to_tile.size();
}

void TileRegistry::clear() {
	id_to_tile.clear();
	key_to_id.clear();
}

uint16_t TileRegistry::getTileId(const std::string &key) const {
	auto it = key_to_id.find(key);
	if (it == key_to_id.end())
		throw std::runtime_error("Tile key not found: " + key);
	return it->second;
}

const Tile &TileRegistry::getTile(const std::string &key) const {
	return getTile(getTileId(key));
}

const Tile &TileRegistry::getTile(uint16_t tile_id) const {
	if (tile_id >= id_to_tile.size())
		throw std::runtime_error("Tile ID out of range");
	return id_to_tile[tile_id];
}

TileRegistry::TileRegistry() {
	registerTile("core:empty", Tile(SliceUV{}, false));
}

}  // namespace lili
