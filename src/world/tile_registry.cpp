#include "lili2d/world/tile_registry.hpp"

namespace lili {

TileRegistry &TileRegistry::get() {
	static TileRegistry instance;
	return instance;
}

uint16_t TileRegistry::registerTile(const std::string &key, Tile &&tile) {
	return registerAsset(key, std::move(tile));
}

bool TileRegistry::hasTile(const std::string &key) const {
	return hasAsset(key);
}

size_t TileRegistry::tileCount() const {
	return assetCount();
}

uint16_t TileRegistry::getTileID(const std::string &key) const {
	return getAssetID(key);
}

const Tile &TileRegistry::getTile(const std::string &key) const {
	return getAsset(key);
}

const Tile &TileRegistry::getTile(uint16_t tile_id) const {
	return getAsset(tile_id);
}

TileRegistry::TileRegistry() {
	registerTile("core:empty", Tile(SliceUV{}, false));
}

}  // namespace lili
