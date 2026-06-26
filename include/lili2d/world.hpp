#pragma once

/// @file world.hpp
/// @brief Aggregates the world components of the Lili2D engine, including
/// camera, chunk, tile, tile registry, tilemap.

#include "lili2d/world/camera.hpp"  // IWYU pragma: keep
#include "lili2d/world/tile.hpp"  // IWYU pragma: keep
#include "lili2d/world/tile_registry.hpp"  // IWYU pragma: keep
#include "lili2d/world/tilemap.hpp"  // IWYU pragma: keep

namespace lili {

/// @brief Defines a camera for viewing the scene.
class Camera;

struct Tile;
class TileRegistry;
class TileMap;

}
