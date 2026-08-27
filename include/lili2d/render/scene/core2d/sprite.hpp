#pragma once

#include <SDL3/SDL_gpu.h>

#include <memory>
#include <string>

#include "lili2d/geometry/utils.hpp"
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
    Sprite() noexcept = default;
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
    Sprite(Sprite&&) noexcept = default;
    /// @brief Move assignment operator.
    /// @return Reference to the assigned sprite.
    Sprite&
    operator=(Sprite&&) noexcept = default;

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
    inline void
    setColorTint(Vec4 color) noexcept {
        if (material) material->properties.color_tint = color;
    }

    /// @brief Sets the sprite's color tint.
    /// @param color The new color tint.
    inline void
    setColor(Vec4 color) noexcept override {
        setColorTint(color);
    }

    /// @brief Sets the material pointer.
    /// @param mat Material pointer.
    inline void
    setMaterial(Material* mat) noexcept override {
        external_material = mat;
    }

    /// @brief Sets the sprite's position.
    /// @param pos The new position.
    inline void
    setPosition(Vec2 pos) noexcept override {
        this->position = pos;
        ui_layout.offset = pos;
    }

    /// @brief Sets the sprite's scale.
    /// @param s The new scale.
    inline void
    setScale(Vec2 s) noexcept override {
        this->scale = s;
    }

    /// @brief Sets the sprite's size (in px).
    /// @param s The new size.
    inline void
    setSize(Vec2 s) noexcept override {
        this->size = s;
    }

    /// @brief Sets the sprite's rotation.
    /// @param degree The rotation in degrees.
    inline void
    setRotation(float degree) noexcept override {
        rotation = lili::degToRad(degree);
    }

    /// @brief Sets the sprite's rendering layer depth.
    /// @param l The new layer depth.
    inline void
    setLayer(float l) noexcept override {
        this->layer = l;
    }

    /// @brief Get the position of the sprite.
    /// @return The position.
    [[nodiscard]] inline Vec2
    getPosition() const noexcept override {
        return position;
    }

    /// @brief Get rotation in degrees.
    /// @return Rotation in degrees.
    [[nodiscard]] inline float
    getRotation() const noexcept override {
        return lili::radToDeg(rotation);
    }

    /// @brief Get scale factors.
    /// @return Scale vector.
    [[nodiscard]] inline Vec2
    getScale() const noexcept override {
        return scale;
    }

    /// @brief Get the render width of the sprite.
    /// @return The width.
    [[nodiscard]] inline float
    getWidth() const noexcept {
        return size.x * scale.x;
    }

    /// @brief Get the render height of the sprite.
    /// @return The height.
    [[nodiscard]] inline float
    getHeight() const noexcept {
        return size.y * scale.y;
    }

    /// @brief Get the render size of the sprite.
    /// @return A 2D vector of the width and height.
    [[nodiscard]] inline Vec2
    getSize() const noexcept override {
        return Vec2(size.x * scale.x, size.y * scale.y);
    }

    /// @brief Gets the transformation matrix.
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

    /// @brief Gets the material.
    /// @return Pointer to the material.
    [[nodiscard]] inline Material*
    getMaterial() const noexcept override {
        return external_material ? external_material : material.get();
    }

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
