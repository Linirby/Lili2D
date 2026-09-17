#include "lili2d/world/tile_registry.hpp"

namespace lili {

TileRegistry&
TileRegistry::get() {
    static TileRegistry instance;
    return instance;
}

TileRegistry::TileRegistry() { registerTile("core:none", Tile()); }

uint16_t
TileRegistry::registerTile(const std::string& key, Tile&& tile) {
    return registerAsset(key, std::move(tile));
}

}  // namespace lili
