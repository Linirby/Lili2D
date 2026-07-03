#pragma once

#include "lili2d/world/tile.hpp"
#include "lili2d/core/asset_registry.hpp"

namespace lili {

/// @brief Central registry for defining and storing tile types.
class TileRegistry : public AssetRegistry<Tile, uint16_t> {
public:
	TileRegistry(const TileRegistry&) = delete;
	TileRegistry &operator=(const TileRegistry &) = delete;

	/// @brief Gets the singleton instance.
	/// @return Reference to the TileRegistry.
	static TileRegistry &get();

	/// @brief Registers a new tile.
	/// @param key The unique string key.
	/// @param tile The tile to register.
	/// @return The assigned tile ID.
	uint16_t registerTile(const std::string &key, Tile &&tile);
	/// @brief Checks if a tile exists.
	/// @param key The tile key.
	/// @return True if the tile exists.
	bool hasTile(const std::string &key) const;
	/// @brief Gets the total number of registered tiles.
	/// @return The number of tiles.
	size_t tileCount() const;
	/// @brief Gets a tile ID by key.
	/// @param key The tile key.
	/// @return The tile ID.
	uint16_t getTileID(const std::string &key) const;
	/// @brief Gets a tile by key.
	/// @param key The tile key.
	/// @return Reference to the tile.
	const Tile &getTile(const std::string &key) const;
	/// @brief Gets a tile by 16-bit ID.
	/// @param tile_id The tile ID.
	/// @return Reference to the tile.
	const Tile &getTile(uint16_t tile_id) const;

private:
	TileRegistry();
};

}  // namespace lili
