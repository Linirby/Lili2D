#pragma once

#include <memory>

#include "lili2d/geometry/vec2.hpp"
#include "lili2d/render/core/gpu_mesh.hpp"
#include "lili2d/render/interfaces/renderable.hpp"
#include "lili2d/render/renderer.hpp"
#include "lili2d/render/scene/common/atlas_map.hpp"
#include "lili2d/render/scene/common/material.hpp"

namespace lili {

/// @brief Batches multiple sprites into a single draw call.
class SpriteBatch : public IRenderable {
public:
    /// @brief Constructor.
    /// @param renderer The renderer.
    /// @param texture The texture to use for the batch.
    SpriteBatch(Renderer* renderer, Texture* texture);
    /// @brief Destructor.
    ~SpriteBatch() override;
    /// @brief Deleted copy constructor.
    SpriteBatch(const SpriteBatch&) = delete;
    /// @brief Deleted copy assignment operator.
    SpriteBatch&
    operator=(const SpriteBatch&) = delete;
    /// @brief Deleted move constructor.
    SpriteBatch(SpriteBatch&&) = delete;
    /// @brief Deleted move assignment operator.
    SpriteBatch&
    operator=(SpriteBatch&&) = delete;

    /// @brief Begins a new batch, clearing previous data.
    void
    begin();

    /// @brief Static helper to append a sprite's vertex data to a MeshData
    /// struct.
    /// @param mesh_data The mesh data to append to.
    /// @param slice The texture slice (UVs) to use.
    /// @param position The local position.
    /// @param scale The local scale.
    /// @param rotation The local rotation in degrees.
    /// @param color The color tint for the vertices.
    static void
    appendSpriteToMesh(
        MeshData& mesh_data, const SliceUV& slice, const Vec2& position,
        const Vec2& scale = {1.0f, 1.0f}, float rotation = 0.0f,
        const Vec4& color = {1.0f, 1.0f, 1.0f, 1.0f}
    );

    /// @brief Directly updates the GPU mesh with the provided CPU mesh data.
    /// @param data The new CPU mesh data.
    void
    setMeshData(MeshData&& data);

    /// @brief Clears all sprites from the batch.
    void
    clear();

    /// @brief Adds a sprite to the batch.
    /// @param slice The texture slice (UVs) to use.
    /// @param position The local position.
    /// @param scale The local scale.
    /// @param rotation The local rotation in degrees.
    /// @param color The color tint for the vertices.
    void
    draw(
        const SliceUV& slice, const Vec2& position,
        const Vec2& scale = {1.0f, 1.0f}, float rotation = 0.0f,
        const Vec4& color = {1.0f, 1.0f, 1.0f, 1.0f}
    );

    /// @brief Ends the batch and updates the GPU mesh.
    void
    end();

    /// @brief Sets the color tint for the entire batch.
    /// @param color The new color tint.
    void
    setColorTint(const Vec4& color);
    /// @brief Sets color tint for the batch.
    /// @param color The new color tint.
    void
    setColor(Vec4 color) override;
    /// @brief Sets material for the batch.
    /// @param material Pointer to material.
    void
    setMaterial(Material* material) override;

    /// @brief Sets the position of the entire batch.
    /// @param position The new position.
    void
    setPosition(Vec2 position) override;
    /// @brief Sets the rotation of the entire batch in degrees.
    /// @param degree Rotation angle in degrees.
    void
    setRotation(float degree) override;
    /// @brief Sets the scale of the entire batch.
    /// @param scale The new scale.
    void
    setScale(Vec2 scale) override;
    /// @brief Overrides the computed bounds size of the batch.
    /// @param size Custom bounds size.
    void
    setSize(Vec2 size) override;

    /// @brief Sets the rendering layer depth for the batch.
    /// @param layer The new layer depth.
    void
    setLayer(float layer) override;

    /// @brief Gets position of batch.
    /// @return Position vector.
    Vec2
    getPosition() const override;
    /// @brief Gets rotation angle in degrees.
    /// @return Rotation in degrees.
    float
    getRotation() const override;
    /// @brief Gets scale.
    /// @return Scale vector.
    Vec2
    getScale() const override;
    /// @brief Gets bounding size of batch mesh.
    /// @return Size vector.
    Vec2
    getSize() const override;
    /// @brief Gets transformation matrix.
    /// @return Mat3 matrix.
    Mat3
    getTransformMatrix() const override;
    /// @brief Gets layer depth.
    /// @return Depth layer.
    float
    getLayer() const override;
    /// @brief Gets color tint.
    /// @return Color tint.
    Vec4
    getColor() const override;
    /// @brief Gets material.
    /// @return Material pointer.
    Material*
    getMaterial() const override;

    /// @brief Submits the batched mesh to the renderer.
    void
    draw() override;

private:
    /// @brief Pointer to parent Renderer.
    Renderer* renderer = nullptr;
    /// @brief Unique pointer to owned Material.
    std::unique_ptr<Material> material = nullptr;
    /// @brief Pointer to external unowned Material.
    Material* external_material = nullptr;
    /// @brief Unique pointer to GPU mesh.
    std::unique_ptr<GPUMesh> mesh = nullptr;

    /// @brief Accumulated CPU mesh data for batching.
    MeshData mesh_data;
    /// @brief Batch position vector.
    Vec2 position;
    /// @brief Batch scale vector.
    Vec2 scale = {1.0f, 1.0f};
    /// @brief Custom bounding size vector.
    Vec2 custom_size = {0.0f, 0.0f};
    /// @brief Batch rotation angle in degrees.
    float rotation = 0.0f;
    /// @brief Render layer depth.
    float layer = 0.0f;
};

}  // namespace lili
