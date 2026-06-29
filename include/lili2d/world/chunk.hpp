#pragma once

#include <vector>
#include <unordered_map>
#include <memory>

#include "lili2d/render/scene/core2d/sprite_batch.hpp"
#include "lili2d/geometry/point3.hpp"
#include "lili2d/render/core/texture.hpp"

namespace lili {

struct BatchKey {
	Texture* texture = nullptr;
	int z = 0;

    bool operator==(const BatchKey &other) const;
};

struct BatchKeyHash {
	std::size_t operator()(const BatchKey &k) const;
};

struct Chunk {
	static constexpr int SIZE = 32;
	std::vector<uint16_t> tiles;
	mutable bool dirty = true;
	mutable std::unordered_map<
		BatchKey, std::unique_ptr<SpriteBatch>, BatchKeyHash
	> batches;

	Chunk();
	Chunk(Chunk &&other) noexcept = default;
	Chunk &operator=(Chunk &&other) noexcept = default;

	Chunk(const Chunk &) = delete;
	Chunk &operator=(const Chunk &) = delete;

	static size_t flattenIndex(lili::Point3 local_pos);
	void rebuildBatches(
		Renderer *renderer, Point3 chunk_pos, const Vec2 &tile_size
	) const;
};

}  // namespace lili
