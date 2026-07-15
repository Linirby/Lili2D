#pragma once

#include <vector>
#include <unordered_map>
#include <memory>
#include <future>

#include "lili2d/render/scene/core2d/sprite_batch.hpp"
#include "lili2d/geometry/point3.hpp"
#include "lili2d/render/core/texture.hpp"

namespace lili {

/// @brief A simple, lightweight C++20 Thread Pool using jthread and stop_token.
class ThreadPool;

/// @brief Key used to group tiles by texture and depth for batching.
struct BatchKey {
	Texture* texture = nullptr;
	int z = 0;

	bool operator==(const BatchKey &other) const;
};

/// @brief Hash function for BatchKey.
struct BatchKeyHash {
	/// @brief Calculates the hash for a BatchKey.
	/// @param k The BatchKey.
	/// @return The hash value.
	std::size_t operator()(const BatchKey &k) const;
};

/// @brief Struct to hold pre-calculated CPU mesh data for all batches of a
/// chunk.
struct ChunkMeshData {
	/// @brief Struct holding the batch key and corresponding raw CPU mesh data.
	struct BatchMeshData {
		BatchKey key;         ///< The batch key.
		MeshData mesh_data;   ///< The CPU-side mesh data.
	};
	std::vector<BatchMeshData> batches; ///< List of batch mesh data.
};

/// @brief Represents a block of tiles in the world.
struct Chunk {
	static constexpr int SIZE = 32;
	std::vector<uint16_t> tiles;
	mutable bool dirty = true;
	mutable bool rebuilding = false;
	mutable std::future<ChunkMeshData> rebuild_future;
	mutable std::unordered_map<
		BatchKey, std::unique_ptr<SpriteBatch>, BatchKeyHash
	> batches;

	Chunk();
	Chunk(Chunk &&other) noexcept = default;
	Chunk &operator=(Chunk &&other) noexcept = default;

	Chunk(const Chunk &) = delete;
	Chunk &operator=(const Chunk &) = delete;

	/// @brief Flattens a 3D local position into a 1D index.
	/// @param local_pos The local 3D position within the chunk.
	/// @return The 1D index.
	static size_t flattenIndex(lili::Point3 local_pos);

	/// @brief CPU-only function to generate mesh data. Thread-safe, no GPU/SDL3
	/// calls.
	/// @param chunk_pos The world position of the chunk.
	/// @param tile_size The size of a single tile.
	/// @param chunk_tiles A copy of the tile vector to prevent data races.
	/// @return The generated CPU-side mesh data.
	ChunkMeshData generateMeshData(
		Point3 chunk_pos,
		const Vec2 &tile_size,
		const std::vector<uint16_t> &chunk_tiles
	) const;

	/// @brief Main thread function to upload generated mesh data to the GPU.
	/// @param renderer The renderer.
	/// @param mesh_data The generated CPU-side mesh data to upload.
	void uploadMeshData(Renderer *renderer, ChunkMeshData &&mesh_data) const;

	/// @brief Rebuilds the sprite batches for rendering the chunk.
	/// @param renderer The renderer.
	/// @param chunk_pos The world position of the chunk.
	/// @param tile_size The size of a single tile.
	void rebuildBatches(
		Renderer *renderer,
		ThreadPool *thread_pool,
		Point3 chunk_pos,
		const Vec2 &tile_size
	) const;
};

}  // namespace lili
