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
	/// @brief Pointer to the texture of the batch.
	Texture* texture = nullptr;
	/// @brief The Z layer/depth level of the batch.
	int z = 0;

	/// @brief Compares this BatchKey with another for equality.
	/// @param other The other BatchKey to compare against.
	/// @return True if the texture and z layer are equal.
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
	/// @brief The width/height/depth size of a chunk in tiles.
	static constexpr int SIZE = 32;
	/// @brief Flat vector of tile IDs belonging to this chunk.
	std::vector<uint16_t> tiles;
	/// @brief Indicates if the chunk needs a mesh rebuild.
	mutable bool dirty = true;
	/// @brief Indicates if a background rebuild task is currently running.
	mutable bool rebuilding = false;
	/// @brief Future object tracking the asynchronous rebuild task.
	mutable std::future<ChunkMeshData> rebuild_future;
	/// @brief Map of sprite batches categorized by their batch key.
	mutable std::unordered_map<
		BatchKey, std::unique_ptr<SpriteBatch>, BatchKeyHash
	> batches;

	/// @brief Default constructor.
	Chunk();
	/// @brief Move constructor.
	Chunk(Chunk &&other) noexcept = default;
	/// @brief Move assignment operator.
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
	/// @param thread_pool The ThreadPool to submit rebuild tasks to.
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
