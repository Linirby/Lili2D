#pragma once

#include "lili2d/geometry/vec2.hpp"
#include "lili2d/render/2d/bitmap_font.hpp"
#include <cstdint>
#include <type_traits>

namespace lili {

/// @brief Component representing 2D text rendered with a BitmapFont and
/// StringArena.
struct cText
{
    BitmapFont* font = nullptr; ///< Font :P
    Vec2 glyph_size = {};       ///< Size of a glyph
    float spacing = 1.0f;       ///< Spacing between letters
    uint32_t offset = 0;        ///< Start index in StringArena
    uint32_t length = 0;        ///< Current string lenght (excluding '\0')
    uint32_t capacity = 0;      ///< Max capacity of the text
};

static_assert(
    std::is_trivially_copyable_v<cText>,
    "cText must be trivially copyable"
);

} // namespace lili
