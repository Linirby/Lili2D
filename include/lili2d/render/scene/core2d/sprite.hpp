#pragma once

#include <SDL3/SDL_gpu.h>

#include <memory>
#include <string>

#include "lili2d/geometry/vec2.hpp"
#include "lili2d/render/core/texture.hpp"
#include "lili2d/render/interfaces/renderable.hpp"
#include "lili2d/render/renderer.hpp"
#include "lili2d/render/scene/common/atlas_map.hpp"
#include "lili2d/render/scene/common/material.hpp"

namespace lili {

/// @brief Represents a renderable 2D sprite.
class Sprite : public IRenderable {
public:
    /// @brief Default constructor.
    Sprite() = default;
    /// @brief Constructs a sprite from an image file.
    /// @param renderer The renderer.
    /// @param path Path to the image file.
    explicit Sprite(Renderer* renderer, const std::string& path);
    /// @brief Constructs a sprite from a UV slice.
    /// @param renderer The renderer.
    /// @param slice The UV slice of an atlas.
    explicit Sprite(Renderer* renderer, const SliceUV& slice);
    /// @brief Constructs a sprite from a Texture.
    /// @param renderer The renderer.
    /// @param texture The texture.
    explicit Sprite(Renderer* renderer, Texture* texture);
    /// @brief Destructor.
    ~Sprite() override = default;

    /// @brief Move constructor.
    Sprite(Sprite&&) = default;
    /// @brief Move assignment operator.
    /// @return Reference to the assigned sprite.
    Sprite&
    operator=(Sprite&&) = default;

    /// @brief Sets the sprite's image.
    /// @param path Path to the new image file.
    void
    setImage(const std::string& path);
    /// @brief Sets the sprite's slice from an atlas map.
    /// @param slice The new UV slice.
    void
    setSlice(const SliceUV& slice);
    /// @brief Sets the sprite's color tint.
    /// @param color The new color tint.
    void
    setColorTint(Vec4 color);
    /// @brief Sets the sprite's color tint.
    /// @param color The new color tint.
    void
    setColor(Vec4 color) override;
    /// @brief Sets the material pointer.
    /// @param material Material pointer.
    void
    setMaterial(Material* material) override;
    /// @brief Sets the sprite's position.
    /// @param position The new position.
    void
    setPosition(Vec2 position) override;
    /// @brief Sets the sprite's scale.
    /// @param scale The new scale.
    void
    setScale(Vec2 scale) override;
    /// @brief Sets the sprite's size (in px).
    /// @param size The new size.
    void
    setSize(Vec2 size) override;
    /// @brief Sets the sprite's rotation.
    /// @param degree The rotation in degrees.
    void
    setRotation(float degree) override;
    /// @brief Sets the sprite's rendering layer depth.
    /// @param layer The new layer depth.
    void
    setLayer(float layer) override;

    /// @brief Get the position of the sprite.
    /// @return The position.
    Vec2
    getPosition() const override;
    /// @brief Get rotation in degrees.
    /// @return Rotation in degrees.
    float
    getRotation() const override;
    /// @brief Get scale factors.
    /// @return Scale vector.
    Vec2
    getScale() const override;
    /// @brief Get the render width of the sprite.
    /// @return The width.
    float
    getWidth() const;
    /// @brief Get the render height of the sprite.
    /// @return The height.
    float
    getHeight() const;
    /// @brief Get the render size of the sprite.
    /// @return A 2D vector of the width and height.
    Vec2
    getSize() const override;
    /// @brief Gets the transformation matrix.
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
    /// @brief Gets the material.
    /// @return Pointer to the material.
    Material*
    getMaterial() const override;

    /// @brief Submits the sprite for drawing.
    void
    draw() override;

private:
    /// @brief Pointer to parent Renderer.
    Renderer* renderer = nullptr;

    /// @brief Unique pointer to full texture if owned by Sprite.
    std::unique_ptr<Texture> texture = nullptr;
    /// @brief Pointer to GPU mesh.
    GPUMesh* mesh = nullptr;
    /// @brief Unique pointer to owned Material.
    std::unique_ptr<Material> material = nullptr;
    /// @brief Pointer to external unowned Material.
    Material* external_material = nullptr;

    /// @brief Active UV slice configuration.
    SliceUV current_slice;
    /// @brief Sprite 2D position.
    Vec2 position;
    /// @brief Sprite 2D scale.
    Vec2 scale = Vec2(1, 1);
    /// @brief Sprite 2D render size.
    Vec2 size = Vec2(1, 1);
    /// @brief Sprite rotation in degrees.
    float rotation = 0.0f;
    /// @brief Render layer depth.
    float layer = 0.0f;
};

}  // namespace lili
