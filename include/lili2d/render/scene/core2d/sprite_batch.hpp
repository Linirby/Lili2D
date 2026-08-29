#pragma once

#include <memory>

#include "lili2d/geometry/utils.hpp"
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
    explicit SpriteBatch(Renderer* renderer, Texture* texture);
    /// @brief Destructor.
    ~SpriteBatch() override = default;

    /// @brief Move constructor.
    SpriteBatch(SpriteBatch&&) noexcept = default;
    /// @brief Move assignment operator.
    SpriteBatch&
    operator=(SpriteBatch&&) noexcept = default;

    /// @brief Deleted copy constructor.
    SpriteBatch(const SpriteBatch&) = delete;
    /// @brief Deleted copy assignment operator.
    SpriteBatch&
    operator=(const SpriteBatch&) = delete;

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
        MeshData& mesh_data, const SliceUV& slice, Vec2 position,
        Vec2 scale = {1.0f, 1.0f}, float rotation = 0.0f,
        Vec4 color = {1.0f, 1.0f, 1.0f, 1.0f}
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
        const SliceUV& slice, Vec2 position, Vec2 scale = {1.0f, 1.0f},
        float rotation = 0.0f, Vec4 color = {1.0f, 1.0f, 1.0f, 1.0f}
    );

    /// @brief Ends the batch and updates the GPU mesh.
    void
    end();

    /// @brief Sets the color tint for the entire batch.
    /// @param color The new color tint.
    inline void
    setColorTint(Vec4 color) noexcept {
        if (material) material->properties.color_tint = color;
    }

    /// @brief Sets color tint for the batch.
    /// @param color The new color tint.
    inline void
    setColor(Vec4 color) noexcept override {
        setColorTint(color);
    }

    /// @brief Sets material for the batch.
    /// @param mat Pointer to material.
    inline void
    setMaterial(Material* mat) noexcept override {
        external_material = mat;
    }

    /// @brief Sets the position of the entire batch.
    /// @param pos The new position.
    inline void
    setPosition(Vec2 pos) noexcept override {
        this->position = pos;
        ui_layout.offset = pos;
    }

    /// @brief Sets the rotation of the entire batch in degrees.
    /// @param degree Rotation angle in degrees.
    inline void
    setRotation(float degree) noexcept override {
        rotation = lili::degToRad(degree);
    }

    /// @brief Sets the scale of the entire batch.
    /// @param s The new scale.
    inline void
    setScale(Vec2 s) noexcept override {
        this->scale = s;
    }

    /// @brief Overrides the computed bounds size of the batch.
    /// @param size Custom bounds size.
    inline void
    setSize(Vec2 size) noexcept override {
        custom_size = size;
    }

    /// @brief Sets the rendering layer depth for the batch.
    /// @param l The new layer depth.
    inline void
    setLayer(float l) noexcept override {
        this->layer = l;
    }

    /// @brief Gets position of batch.
    /// @return Position vector.
    [[nodiscard]] inline Vec2
    getPosition() const noexcept override {
        return position;
    }

    /// @brief Gets rotation angle in degrees.
    /// @return Rotation in degrees.
    [[nodiscard]] inline float
    getRotation() const noexcept override {
        return lili::radToDeg(rotation);
    }

    /// @brief Gets scale.
    /// @return Scale vector.
    [[nodiscard]] inline Vec2
    getScale() const noexcept override {
        return scale;
    }

    /// @brief Gets bounding size of batch mesh.
    /// @return Size vector.
    [[nodiscard]] Vec2
    getSize() const noexcept override;

    /// @brief Gets transformation matrix.
    /// @return Mat3 matrix.
    [[nodiscard]] Mat3
    getTransformMatrix() const override;

    /// @brief Gets layer depth.
    /// @return Depth layer.
    [[nodiscard]] inline float
    getLayer() const noexcept override {
        return layer;
    }

    /// @brief Gets color tint.
    /// @return Color tint.
    [[nodiscard]] inline Vec4
    getColor() const noexcept override {
        Material* mat = getMaterial();
        return mat ? mat->properties.color_tint : Vec4(1.0f, 1.0f, 1.0f, 1.0f);
    }

    /// @brief Gets material.
    /// @return Material pointer.
    [[nodiscard]] inline Material*
    getMaterial() const noexcept override {
        return external_material ? external_material : material.get();
    }

    /// @brief Submits the batched mesh to the renderer.
    void
    draw() override;

private:
    Renderer* renderer = nullptr;
    std::unique_ptr<Material> material = nullptr;
    Material* external_material = nullptr;
    std::unique_ptr<GPUMesh> mesh = nullptr;

    MeshData mesh_data;
    Vec2 position;
    Vec2 scale = {1.0f, 1.0f};
    Vec2 custom_size = {0.0f, 0.0f};
    float rotation = 0.0f;
    float layer = 0.0f;
};

}  // namespace lili
