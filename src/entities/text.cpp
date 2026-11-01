#include "lili2d/entities/text.hpp"
#include "lili2d/components/render2d.hpp"
#include "lili2d/components/text.hpp"
#include "lili2d/ecs/ecs_registry.hpp"
#include "lili2d/ecs/entity.hpp"
#include "lili2d/render/2d/bitmap_font.hpp"
#include <cmath>
#include <cstdint>
#include <string_view>

namespace lili {

namespace {

[[nodiscard]] Vec2
calculateTextSize(BitmapFont* font, std::string_view text, float spacing)
{
    if (!font || text.empty())
        return { 0.0f, 0.0f };

    const float glyph_w = static_cast<float>(font->getGlyphW());
    const float glyph_h = static_cast<float>(font->getGlyphH());
    const float advance = glyph_w + spacing;
    const float abs_advance = std::abs(advance);

    float max_w = 0.0f;
    float line_count = 1.0f;
    size_t line_chars = 0;

    for (char c : text) {
        if (c == '\n') {
            line_count += 1.0f;
            line_chars = 0;
        } else {
            ++line_chars;
            float w =
                glyph_w + static_cast<float>(line_chars - 1) * abs_advance;
            if (w > max_w)
                max_w = w;
        }
    }

    return { max_w, line_count * glyph_h };
}

} // namespace

Entity
createText(ECSRegistry& registry, const TextParameters& params)
{
    Entity entity = registry.createEntity();

    Render2DComponent render2d = params.render2d;
    render2d.size = calculateTextSize(
        params.text.font, params.content, params.text.spacing
    );

    TextComponent text = params.text;
    text.glyph_size = params.text.font ? params.text.font->getGlyphSize()
                                       : Vec2{ 0.0f, 0.0f };
    text.offset = registry.getStringArena().append(params.content);
    text.length = static_cast<uint32_t>(params.content.size());

    registry.addComponents(entity, params.transform, render2d, text);
    return entity;
}

} // namespace lili
