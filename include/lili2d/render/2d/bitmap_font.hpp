#pragma once

#include "lili2d/geometry/vec2.hpp"
#include "lili2d/render/gpu/texture.hpp"
#include "lili2d/render/renderer.hpp"

namespace lili {

/// @brief Defines the UV coordinates for a single glyph in a bitmap font.
struct GlyphUV
{
    float u0 = 0; ///< U0 coordinate.
    float v0 = 0; ///< V0 coordinate.
    float u1 = 0; ///< U1 coordinate.
    float v1 = 0; ///< V1 coordinate.

    /// @brief Default constructor.
    constexpr GlyphUV() noexcept = default;
    /// @brief Constructs GlyphUV with bounds coordinates.
    /// @param u0 Minimum U coordinate.
    /// @param v0 Minimum V coordinate.
    /// @param u1 Maximum U coordinate.
    /// @param v1 Maximum V coordinate.
    constexpr GlyphUV(float u0, float v0, float u1, float v1) noexcept
      : u0(u0)
      , v0(v0)
      , u1(u1)
      , v1(v1)
    {
    }
};

/// @brief Represents a bitmap font loaded from an image.
class BitmapFont
{
public:
    /// @brief Default constructor.
    BitmapFont() = default;
    /// @brief Constructs a bitmap font.
    /// @param renderer The renderer.
    /// @param path Path to the font image.
    /// @param cols Number of columns in the font grid.
    /// @param rows Number of rows in the font grid.
    explicit BitmapFont(
        Renderer* renderer,
        const std::string& path,
        uint8_t cols,
        uint8_t rows
    );
    /// @brief Constructs a bitmap font from memory data.
    /// @param renderer The renderer.
    /// @param data Pointer to the image data.
    /// @param len Length of the image data in bytes.
    /// @param cols Number of columns in the font grid.
    /// @param rows Number of rows in the font grid.
    explicit BitmapFont(
        Renderer* renderer,
        const unsigned char* data,
        unsigned int len,
        uint8_t cols,
        uint8_t rows
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
    getTexture() const noexcept
    {
        return texture.get();
    }
    /// @brief Gets the width of a single glyph.
    /// @return The width in pixels.
    [[nodiscard]] inline int
    getGlyphW() const noexcept
    {
        return glyph_w;
    }
    /// @brief Gets the height of a single glyph.
    /// @return The height in pixels.
    [[nodiscard]] inline int
    getGlyphH() const noexcept
    {
        return glyph_h;
    }
    /// @brief Gets the size of a single glyph.
    /// @return The Vec2 with (.x = width, .y = height)
    [[nodiscard]] inline Vec2
    getGlyphSize() const noexcept
    {
        return Vec2(glyph_w, glyph_h);
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

} // namespace lili
