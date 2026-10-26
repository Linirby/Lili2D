#include "lili2d/render/2d/bitmap_font.hpp"

namespace lili {

BitmapFont::BitmapFont(
    Renderer* renderer,
    const std::string& path,
    uint8_t cols,
    uint8_t rows
)
  : cols(cols)
  , rows(rows)
{
    texture = std::make_unique<Texture>(renderer->getDevice(), path);
    this->cols = cols;
    this->rows = rows;
    glyph_w = texture->getWidth() / cols;
    glyph_h = texture->getHeight() / rows;
}

BitmapFont::BitmapFont(
    Renderer* renderer,
    const unsigned char* data,
    unsigned int len,
    uint8_t cols,
    uint8_t rows
)
  : cols(cols)
  , rows(rows)
{
    texture = std::make_unique<Texture>(renderer->getDevice(), data, len);
    this->cols = cols;
    this->rows = rows;
    glyph_w = texture->getWidth() / cols;
    glyph_h = texture->getHeight() / rows;
}

BitmapFont::BitmapFont(BitmapFont&& other) noexcept
  : texture(std::move(other.texture))
  , cols(other.cols)
  , rows(other.rows)
  , glyph_w(other.glyph_w)
  , glyph_h(other.glyph_h)
{
}

BitmapFont&
BitmapFont::operator=(BitmapFont&& other) noexcept
{
    if (this != &other) {
        if (texture && other.texture)
            *texture = std::move(*other.texture);
        else
            texture = std::move(other.texture);
        cols = other.cols;
        rows = other.rows;
        glyph_w = other.glyph_w;
        glyph_h = other.glyph_h;
    }
    return *this;
}

GlyphUV
BitmapFont::glyphUv(char c) const noexcept
{
    const int ASCII = static_cast<unsigned char>(c);
    const int FIRST = static_cast<unsigned char>(' ');
    int idx = ASCII - FIRST;
    if (ASCII < FIRST || ASCII > 126)
        idx = '?' - FIRST;
    const int CURRENT_X = idx % cols;
    const int CURRENT_Y = idx / cols;
    const float DELTA_U = 1.0f / static_cast<float>(cols);
    const float DELTA_V = 1.0f / static_cast<float>(rows);

    float tex_w = static_cast<float>(texture->getWidth());
    float tex_h = static_cast<float>(texture->getHeight());

    float eps_u = 0.5f / tex_w;
    float eps_v = 0.5f / tex_h;

    float u0 = CURRENT_X * DELTA_U + eps_u;
    float v0 = CURRENT_Y * DELTA_V + eps_v;
    float u1 = (CURRENT_X + 1) * DELTA_U - eps_u;
    float v1 = (CURRENT_Y + 1) * DELTA_V - eps_v;

    return GlyphUV(u0, v0, u1, v1);
}

} // namespace lili
