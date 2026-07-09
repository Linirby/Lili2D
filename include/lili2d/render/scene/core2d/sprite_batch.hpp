#pragma once

#include <memory>

#include "lili2d/render/renderer.hpp"
#include "lili2d/render/scene/common/material.hpp"
#include "lili2d/render/core/gpu_mesh.hpp"
#include "lili2d/render/scene/common/atlas_map.hpp"
#include "lili2d/geometry/vec2.hpp"

namespace lili {

/// @brief Batches multiple sprites into a single draw call.
///
/// SpriteBatch is optimized for rendering many tiles or sprites that share
/// the same texture (AtlasMap). Call begin() to start building the batch,
/// draw() to add sprites, and end() to update the GPU mesh. Finally, call
/// render() to submit the batch to the renderer.
class SpriteBatch {
public:
	/// @brief Constructor.
	/// @param renderer The renderer.
	/// @param texture The texture to use for the batch.
	SpriteBatch(Renderer *renderer, Texture *texture);
	/// @brief Destructor.
	~SpriteBatch();
	SpriteBatch(const SpriteBatch&) = delete;
	SpriteBatch& operator=(const SpriteBatch&) = delete;
	SpriteBatch(SpriteBatch&&) = delete;
	SpriteBatch& operator=(SpriteBatch&&) = delete;

	/// @brief Begins a new batch, clearing previous data.
	void begin();

	/// @brief Static helper to append a sprite's vertex data to a MeshData
	/// struct.
	/// @param mesh_data The mesh data to append to.
	/// @param slice The texture slice (UVs) to use.
	/// @param position The local position.
	/// @param scale The local scale.
	/// @param rotation The local rotation in degrees.
	/// @param color The color tint for the vertices.
	static void appendSpriteToMesh(
		MeshData &mesh_data,
		const SliceUV &slice,
		const Vec2 &position,
		const Vec2 &scale = { 1.0f, 1.0f },
		float rotation = 0.0f,
		const Vec4 &color = { 1.0f, 1.0f, 1.0f, 1.0f }
	);

	/// @brief Directly updates the GPU mesh with the provided CPU mesh data.
	/// @param data The new CPU mesh data.
	void setMeshData(MeshData &&data);

	/// @brief Clears all sprites from the batch.
	void clear();

	/// @brief Adds a sprite to the batch.
	/// @param slice The texture slice (UVs) to use.
	/// @param position The local position.
	/// @param scale The local scale.
	/// @param rotation The local rotation in degrees.
	/// @param color The color tint for the vertices.
	void draw(
		const SliceUV &slice,
		const Vec2 &position,
		const Vec2 &scale = { 1.0f, 1.0f },
		float rotation = 0.0f,
		const Vec4 &color = { 1.0f, 1.0f, 1.0f, 1.0f }
	);

	/// @brief Ends the batch and updates the GPU mesh.
	void end();

	/// @brief Sets the color tint for the entire batch.
	/// @param color The new color tint.
	void setColorTint(const Vec4 &color);

	/// @brief Sets the position of the entire batch.
	/// @param position The new position.
	void setPosition(const Vec2 &position);

	/// @brief Sets the scale of the entire batch.
	/// @param scale The new scale.
	void setScale(const Vec2 &scale);

	/// @brief Sets the rendering layer for the batch.
	/// @param layer The new layer depth.
	void setLayer(float layer);

	/// @brief Submits the batched mesh to the renderer.
	void draw();

private:
	Renderer *renderer = nullptr;
	std::unique_ptr<Material> material = nullptr;
	std::unique_ptr<GPUMesh> mesh = nullptr;

	MeshData mesh_data;
	Vec2 position;
	Vec2 scale = { 1.0f, 1.0f };
	float layer;
};

}  // namespace lili
