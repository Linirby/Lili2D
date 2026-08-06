#pragma once

/// @file world.hpp
/// @brief Aggregates the world components of the Lili2D engine, including
/// camera, chunk, tile, tile registry, tilemap.

#include "lili2d/world/camera.hpp"
#include "lili2d/world/chunk.hpp"
#include "lili2d/world/tile.hpp"
#include "lili2d/world/tile_registry.hpp"
#include "lili2d/world/tilemap.hpp"

namespace lili {

/// @brief Key used to group tiles by texture and depth for batching.
struct BatchKey;
/// @brief Hash function for BatchKey.
struct BatchKeyHash;
/// @brief Struct holding the batch key and corresponding raw CPU mesh data.
struct BatchMeshData;
/// @brief Represents a 2D camera for scene viewing.
class Camera;
/// @brief Represents a block of tiles in the world.
struct Chunk;
/// @brief Struct to hold pre-calculated CPU mesh data for all batches of a
/// chunk.
struct ChunkMeshData;
/// @brief Comparison functor for Point3, used in std::map.
struct Point3Compare;
/// @brief Represents a single tile in the world.
struct Tile;
/// @brief Manages a grid-based world of chunks and tiles.
class TileMap;
/// @brief Central registry for defining and storing tile types.
class TileRegistry;

}  // namespace lili
