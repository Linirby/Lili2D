#pragma once

#include <string_view>

#include "lili2d/core/asset_registry.hpp"
#include "lili2d/world/tile.hpp"

namespace lili {

/// @brief Central registry for defining and storing tile types.
class TileRegistry : public AssetRegistry<Tile, uint16_t> {
public:
    /// @brief Deleted copy constructor.
    TileRegistry(const TileRegistry&) = delete;
    /// @brief Deleted copy assignment operator.
    TileRegistry&
    operator=(const TileRegistry&) = delete;

    /// @brief Gets the singleton instance.
    /// @return Reference to the TileRegistry.
    static TileRegistry&
    get();

    /// @brief Registers a new tile.
    /// @param key The unique string key.
    /// @param tile The tile to register.
    /// @return The assigned tile ID.
    inline uint16_t
    registerTile(const std::string& key, Tile&& tile) {
        return registerAsset(key, std::move(tile));
    }
    /// @brief Checks if a tile exists.
    /// @param key The tile key.
    /// @return True if the tile exists.
    [[nodiscard]] inline bool
    hasTile(std::string_view key) const noexcept {
        return hasAsset(key);
    }
    /// @brief Gets the total number of registered tiles.
    /// @return The number of tiles.
    [[nodiscard]] inline size_t
    tileCount() const noexcept {
        return assetCount();
    }
    /// @brief Gets a tile ID by key.
    /// @param key The tile key.
    /// @return The tile ID.
    [[nodiscard]] inline uint16_t
    getTileID(std::string_view key) const {
        return getAssetID(key);
    }
    /// @brief Gets a tile by key.
    /// @param key The tile key.
    /// @return Reference to the tile.
    [[nodiscard]] inline const Tile&
    getTile(std::string_view key) const {
        return getAsset(key);
    }
    /// @brief Gets a tile by 16-bit ID.
    /// @param tile_id The tile ID.
    /// @return Reference to the tile.
    [[nodiscard]] inline const Tile&
    getTile(uint16_t tile_id) const {
        return getAsset(tile_id);
    }

private:
    TileRegistry();
};

}  // namespace lili
