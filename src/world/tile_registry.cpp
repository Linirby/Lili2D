#include "lili2d/world/tile_registry.hpp"

namespace lili {

TileRegistry&
TileRegistry::get() {
    static TileRegistry instance;
    return instance;
}

TileRegistry::TileRegistry() { registerTile("core:none", Tile()); }

}  // namespace lili
