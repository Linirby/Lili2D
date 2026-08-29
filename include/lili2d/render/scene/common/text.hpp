#pragma once

#include <SDL3/SDL_gpu.h>

#include <memory>
#include <string>

#include "lili2d/geometry/utils.hpp"
#include "lili2d/geometry/vec2.hpp"
#include "lili2d/render/core/gpu_mesh.hpp"
#include "lili2d/render/core/texture.hpp"
#include "lili2d/render/interfaces/renderable.hpp"
#include "lili2d/render/renderer.hpp"
#include "lili2d/render/scene/common/material.hpp"
#include "lili2d/render/scene/common/model.hpp"

namespace lili {

/// @brief Defines the UV coordinates for a single glyph in a bitmap font.
struct GlyphUV {
    float u0 = 0;  ///< U0 coordinate.
    float v0 = 0;  ///< V0 coordinate.
    float u1 = 0;  ///< U1 coordinate.
    float v1 = 0;  ///< V1 coordinate.

    /// @brief Default constructor.
    constexpr GlyphUV() noexcept = default;
    /// @brief Constructs GlyphUV with bounds coordinates.
    /// @param u0 Minimum U coordinate.
    /// @param v0 Minimum V coordinate.
    /// @param u1 Maximum U coordinate.
    /// @param v1 Maximum V coordinate.
    constexpr GlyphUV(float u0, float v0, float u1, float v1) noexcept
        : u0(u0), v0(v0), u1(u1), v1(v1) {}
};

/// @brief Represents a bitmap font loaded from an image.
class BitmapFont {
public:
    /// @brief Default constructor.
    BitmapFont() = default;
    /// @brief Constructs a bitmap font.
    /// @param renderer The renderer.
    /// @param path Path to the font image.
    /// @param cols Number of columns in the font grid.
    /// @param rows Number of rows in the font grid.
    explicit BitmapFont(
        Renderer* renderer, const std::string& path, uint8_t cols, uint8_t rows
    );

    /// @brief Move constructor.
    BitmapFont(BitmapFont&& other) noexcept;
    /// @brief Move assignment operator.
    /// @return Reference to assigned BitmapFont.
    BitmapFont&
    operator=(BitmapFont&& other) noexcept;

    /// @brief Gets the font's underlying texture.
    /// @return Pointer to the texture.
    [[nodiscard]] inline Texture*
    getTexture() const noexcept {
        return texture.get();
    }
    /// @brief Gets the width of a single glyph.
    /// @return The width in pixels.
    [[nodiscard]] inline int
    getGlyphW() const noexcept {
        return glyph_w;
    }
    /// @brief Gets the height of a single glyph.
    /// @return The height in pixels.
    [[nodiscard]] inline int
    getGlyphH() const noexcept {
        return glyph_h;
    }
    /// @brief Calculates the UV coordinates for a specific character.
    /// @param c The character.
    /// @return The UV coordinates.
    [[nodiscard]] GlyphUV
    glyphUv(char c) const noexcept;

private:
    std::unique_ptr<Texture> texture = nullptr;
    int cols = 1;
    int rows = 1;

    int glyph_w = 1;
    int glyph_h = 1;
};

/// @brief Renderable Text using a bitmap font.
class Text : public IRenderable {
public:
    /// @brief Default constructor.
    Text() noexcept = default;
    /// @brief Constructs text using a bitmap font.
    /// @param renderer The renderer.
    /// @param font Pointer to the bitmap font.
    /// @param text The text string to render.
    explicit Text(
        Renderer* renderer, BitmapFont* font, const std::string& text
    );
    /// @brief Destructor.
    ~Text() override = default;

    /// @brief Move constructor.
    Text(Text&&) noexcept = default;
    /// @brief Move assignment operator.
    /// @return Reference to the assigned text.
    Text&
    operator=(Text&&) noexcept = default;

    /// @brief Sets the text string.
    /// @param value The new text string.
    void
    setText(const std::string& value);
    /// @brief Sets the text's position.
    /// @param position The new position.
    inline void
    setPosition(Vec2 position) noexcept override {
        pos = position;
        ui_layout.offset = position;
    }
    /// @brief Sets rotation in degrees.
    /// @param degree Rotation in degrees.
    inline void
    setRotation(float degree) noexcept override {
        rotation = lili::degToRad(degree);
    }
    /// @brief Sets scale factors.
    /// @param scale The new scale.
    inline void
    setScale(Vec2 scale) noexcept override {
        this->scale = scale;
    }
    /// @brief Sets uniform scale.
    /// @param value The scale value.
    inline void
    setScale(float value) noexcept {
        this->scale = Vec2(value, value);
    }
    /// @brief Sets text bounds size.
    /// @param size The size.
    inline void
    setSize(Vec2 size) noexcept override {
        (void)size;
    }
    /// @brief Sets the spacing between characters.
    /// @param value The spacing value.
    void
    setSpacing(float value);
    /// @brief Sets text color tint.
    /// @param color Color tint.
    inline void
    setColor(Vec4 color) noexcept override {
        if (material) {
            material->properties.color_tint = color;
        }
    }
    /// @brief Sets material.
    /// @param material Material pointer.
    inline void
    setMaterial(Material* material) noexcept override {
        external_material = material;
    }
    /// @brief Sets layer depth.
    /// @param layer Layer depth.
    inline void
    setLayer(float layer) noexcept override {
        this->layer = layer;
    }

    /// @brief Gets position.
    /// @return Position.
    [[nodiscard]] inline Vec2
    getPosition() const noexcept override {
        return pos;
    }
    /// @brief Gets rotation in degrees.
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
    /// @brief Gets bounding size of text.
    /// @return Size vector.
    [[nodiscard]] inline Vec2
    getSize() const noexcept override {
        float width = text.length() * advance * scale.x;
        float height = glyph_h * scale.y;
        return {width, height};
    }
    /// @brief Gets transformation matrix.
    /// @return Mat3 transform matrix.
    [[nodiscard]] Mat3
    getTransformMatrix() const override;
    /// @brief Gets layer depth.
    /// @return Depth layer.
    [[nodiscard]] inline float
    getLayer() const noexcept override {
        return layer;
    }
    /// @brief Gets color tint.
    /// @return Color tint vector.
    [[nodiscard]] inline Vec4
    getColor() const noexcept override {
        Material* mat = getMaterial();
        return mat ? mat->properties.color_tint : Vec4(1, 1, 1, 1);
    }
    /// @brief Gets the material.
    /// @return Pointer to the material.
    [[nodiscard]] inline Material*
    getMaterial() const noexcept override {
        return external_material ? external_material : material.get();
    }

    /// @brief Submits the text for drawing.
    void
    draw() override;

private:
    Renderer* renderer = nullptr;
    BitmapFont* font = nullptr;

    std::string text = "default";
    Vec2 pos;

    float rotation = 0.0f;
    Vec2 scale = {1.0f, 1.0f};
    float glyph_w = 1.0f;
    float glyph_h = 1.0f;
    float advance = 1.0f;

    std::unique_ptr<Material> material = nullptr;
    Material* external_material = nullptr;
    std::unique_ptr<GPUMesh> mesh = nullptr;
    Model model;
    float layer = 0.0f;

    void
    rebuildMesh();
};

}  // namespace lili
