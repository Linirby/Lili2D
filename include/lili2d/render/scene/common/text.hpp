#pragma once

#include <SDL3/SDL_gpu.h>

#include <memory>
#include <string>

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
    GlyphUV() = default;
    /// @brief Constructs GlyphUV with bounds coordinates.
    /// @param u0 Minimum U coordinate.
    /// @param v0 Minimum V coordinate.
    /// @param u1 Maximum U coordinate.
    /// @param v1 Maximum V coordinate.
    GlyphUV(float u0, float v0, float u1, float v1);
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
    Texture*
    getTexture() const;
    /// @brief Gets the width of a single glyph.
    /// @return The width in pixels.
    int
    getGlyphW() const;
    /// @brief Gets the height of a single glyph.
    /// @return The height in pixels.
    int
    getGlyphH() const;
    /// @brief Calculates the UV coordinates for a specific character.
    /// @param c The character.
    /// @return The UV coordinates.
    GlyphUV
    glyphUv(char c) const;

private:
    /// @brief Unique pointer to the font texture.
    std::unique_ptr<Texture> texture = nullptr;
    /// @brief Number of grid columns.
    int cols = 1;
    /// @brief Number of grid rows.
    int rows = 1;

    /// @brief Single glyph width in pixels.
    int glyph_w = 1;
    /// @brief Single glyph height in pixels.
    int glyph_h = 1;
};

/// @brief Represents renderable text using a bitmap font.
class Text : public IRenderable {
public:
    /// @brief Default constructor.
    Text() = default;
    /// @brief Constructs text using a bitmap font.
    /// @param renderer The renderer.
    /// @param font Pointer to the bitmap font.
    /// @param text The text string to render.
    explicit Text(Renderer* renderer, BitmapFont* font, const std::string& text);
    /// @brief Destructor.
    ~Text() override = default;

    /// @brief Move constructor.
    Text(Text&&) = default;
    /// @brief Move assignment operator.
    /// @return Reference to the assigned text.
    Text&
    operator=(Text&&) = default;

    /// @brief Sets the text string.
    /// @param value The new text string.
    void
    setText(const std::string& value);
    /// @brief Sets the text's position.
    /// @param position The new position.
    void
    setPosition(Vec2 position) override;
    /// @brief Sets rotation in degrees.
    /// @param degree Rotation in degrees.
    void
    setRotation(float degree) override;
    /// @brief Sets scale factors.
    /// @param scale The new scale.
    void
    setScale(Vec2 scale) override;
    /// @brief Sets uniform scale.
    /// @param value The scale value.
    void
    setScale(float value);
    /// @brief Sets text bounds size.
    /// @param size The size.
    void
    setSize(Vec2 size) override;
    /// @brief Sets the spacing between characters.
    /// @param value The spacing value.
    void
    setSpacing(float value);
    /// @brief Sets text color tint.
    /// @param color Color tint.
    void
    setColor(Vec4 color) override;
    /// @brief Sets material.
    /// @param material Material pointer.
    void
    setMaterial(Material* material) override;
    /// @brief Sets layer depth.
    /// @param layer Layer depth.
    void
    setLayer(float layer) override;

    /// @brief Gets position.
    /// @return Position.
    Vec2
    getPosition() const override;
    /// @brief Gets rotation in degrees.
    /// @return Rotation in degrees.
    float
    getRotation() const override;
    /// @brief Gets scale.
    /// @return Scale vector.
    Vec2
    getScale() const override;
    /// @brief Gets bounding size of text.
    /// @return Size vector.
    Vec2
    getSize() const override;
    /// @brief Gets transformation matrix.
    /// @return Mat3 transform matrix.
    Mat3
    getTransformMatrix() const override;
    /// @brief Gets layer depth.
    /// @return Depth layer.
    float
    getLayer() const override;
    /// @brief Gets color tint.
    /// @return Color tint vector.
    Vec4
    getColor() const override;
    /// @brief Gets the material.
    /// @return Pointer to the material.
    Material*
    getMaterial() const override;

    /// @brief Submits the text for drawing.
    void
    draw() override;

private:
    /// @brief Pointer to the main Renderer.
    Renderer* renderer = nullptr;
    /// @brief Pointer to the bitmap font used.
    BitmapFont* font = nullptr;

    /// @brief Rendered string text content.
    std::string text = "default";
    /// @brief Position vector.
    Vec2 pos;

    /// @brief Rotation angle in degrees.
    float rotation = 0.0f;
    /// @brief Scale vector.
    Vec2 scale = {1.0f, 1.0f};
    /// @brief Calculated glyph width.
    float glyph_w = 1.0f;
    /// @brief Calculated glyph height.
    float glyph_h = 1.0f;
    /// @brief Character advance spacing.
    float advance = 1.0f;

    /// @brief Owned internal Material instance.
    std::unique_ptr<Material> material = nullptr;
    /// @brief External unowned Material pointer.
    Material* external_material = nullptr;
    /// @brief Unique pointer to GPU mesh data.
    std::unique_ptr<GPUMesh> mesh = nullptr;
    /// @brief Model structure combining mesh and material.
    Model model;
    /// @brief Rendering layer depth.
    float layer = 0.0f;

    /// @brief Rebuilds text GPU mesh vertices and indices based on string
    /// content.
    void
    rebuildMesh();
};

}  // namespace lili
