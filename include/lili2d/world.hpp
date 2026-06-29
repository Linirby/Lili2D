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

/// @brief Represents a single tile data structure.
struct Tile;
/// @brief Central registry for defining and storing tile types.
class TileRegistry;
/// @brief Manages a grid-based world of chunks and tiles.
class TileMap;

}  // namespace lili
