#pragma once

#include <future>
#include <memory>
#include <unordered_map>
#include <vector>

#include "lili2d/geometry/point3.hpp"
#include "lili2d/render/core/texture.hpp"
#include "lili2d/render/scene/core2d/sprite_batch.hpp"

namespace lili {

/// @brief A simple, lightweight C++20 Thread Pool using jthread and
/// stop_token.
class ThreadPool;

/// @brief Key used to group tiles by texture and depth for batching.
struct BatchKey {
    Texture* texture = nullptr;  ///< Pointer to texture.
    int z = 0;                   ///< Z layer depth.

    /// @brief Equality comparison operator for BatchKey.
    /// @param other Key to compare against.
    /// @return True if equal, false otherwise.
    [[nodiscard]] constexpr bool
    operator==(BatchKey other) const noexcept {
        return texture == other.texture && z == other.z;
    }
};

/// @brief Hash function for BatchKey.
struct BatchKeyHash {
    /// @brief Calculates the hash for a BatchKey.
    /// @param k The BatchKey.
    /// @return The hash value.
    [[nodiscard]] inline std::size_t
    operator()(BatchKey k) const noexcept {
        return std::hash<void*>{}(k.texture) ^ (std::hash<int>{}(k.z) << 1);
    }
};

/// @brief Struct to hold pre-calculated CPU mesh data for all batches of a
/// chunk.
struct ChunkMeshData {
    /// @brief Struct holding the batch key and corresponding raw CPU mesh
    /// data.
    struct BatchMeshData {
        BatchKey key;        ///< The batch key.
        MeshData mesh_data;  ///< The CPU-side mesh data.
    };
    std::vector<BatchMeshData> batches;  ///< List of batch mesh data.
};

/// @brief Represents a block of tiles in the world.
struct Chunk {
    /// @brief Chunk edge size in tile count (32x32).
    static constexpr int SIZE = 32;
    /// @brief Vector of tile IDs in the chunk.
    std::vector<uint16_t> tiles;
    /// @brief Flag indicating if chunk mesh needs rebuilding.
    mutable bool dirty = true;
    /// @brief Flag indicating if async rebuild is in progress.
    mutable bool rebuilding = false;
    /// @brief Future tracking async mesh generation task.
    mutable std::future<ChunkMeshData> rebuild_future;
    /// @brief Map of batch keys to sprite batch rendering objects.
    mutable std::unordered_map<
        BatchKey, std::unique_ptr<SpriteBatch>, BatchKeyHash>
        batches;

    /// @brief Default constructor.
    Chunk();
    /// @brief Move constructor.
    Chunk(Chunk&& other) noexcept = default;
    /// @brief Move assignment operator.
    /// @return Reference to assigned chunk.
    Chunk&
    operator=(Chunk&& other) noexcept = default;

    /// @brief Deleted copy constructor.
    Chunk(const Chunk&) = delete;
    /// @brief Deleted copy assignment operator.
    Chunk&
    operator=(const Chunk&) = delete;

    /// @brief Flattens a 3D local position into a 1D index.
    /// @param local_pos The local 3D position within the chunk.
    /// @return The 1D index.
    [[nodiscard]] static constexpr inline size_t
    flattenIndex(lili::Point3 local_pos) noexcept {
        return static_cast<size_t>(
            local_pos.x + local_pos.y * SIZE + local_pos.z * SIZE * SIZE
        );
    }

    /// @brief CPU-only function to generate mesh data. Thread-safe, no
    /// GPU/SDL3 calls.
    /// @param chunk_pos The world position of the chunk.
    /// @param tile_size The size of a single tile.
    /// @param chunk_tiles A copy of the tile vector to prevent data races.
    /// @return The generated CPU-side mesh data.
    ChunkMeshData
    generateMeshData(
        Point3 chunk_pos, Vec2 tile_size,
        const std::vector<uint16_t>& chunk_tiles
    ) const;

    /// @brief Main thread function to upload generated mesh data to the GPU.
    /// @param renderer The renderer.
    /// @param mesh_data The generated CPU-side mesh data to upload.
    void
    uploadMeshData(Renderer* renderer, ChunkMeshData&& mesh_data) const;

    /// @brief Rebuilds the sprite batches for rendering the chunk.
    /// @param renderer The renderer.
    /// @param thread_pool Pointer to worker thread pool for async rebuild.
    /// @param chunk_pos The world position of the chunk.
    /// @param tile_size The size of a single tile.
    void
    rebuildBatches(
        Renderer* renderer, ThreadPool* thread_pool, Point3 chunk_pos,
        Vec2 tile_size
    ) const;
};

}  // namespace lili
