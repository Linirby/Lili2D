#include "lili2d/entities/text.hpp"
#include "lili2d/components/render2d.hpp"
#include "lili2d/components/text.hpp"
#include "lili2d/components/transform.hpp"
#include "lili2d/ecs/ecs_registry.hpp"
#include "lili2d/ecs/entity.hpp"
#include "lili2d/render/2d/bitmap_font.hpp"
#include <cstdint>
#include <string_view>

namespace lili {

namespace {

[[nodiscard]] Vec2
calculateTextSize(BitmapFont* font, std::string_view text, float spacing)
{
    if (!font || text.empty())
        return { 0.0f, 0.0f };

    float glyph_w = static_cast<float>(font->getGlyphW());
    float glyph_h = static_cast<float>(font->getGlyphH());
    float max_w = 0.0f;
    float current_w = 0.0f;
    float lines = 1.0f;

    for (size_t i = 0; i < text.size(); ++i) {
        char c = text[i];
        if (c == '\n') {
            lines += 1.0f;
            current_w = 0.0f;
        } else {
            current_w += glyph_w;
            if (i + 1 < text.size() && text[i + 1] != '\n')
                current_w += spacing;
            if (current_w > max_w)
                max_w = current_w;
        }
    }
    return { max_w, lines * glyph_h };
}

} // namespace

Entity
createText(
    ECSRegistry& registry,
    BitmapFont* font,
    const TextParameters& params
)
{
    Entity entity = registry.createEntity();

    uint32_t offset = registry.getStringArena().append(params.text);
    uint32_t length = static_cast<uint32_t>(params.text.size());

    Vec2 glyph_size = font ? font->getGlyphSize() : Vec2{ 0.0f, 0.0f };
    Vec2 text_size = calculateTextSize(font, params.text, params.spacing);

    TransformComponent trans{
        .pos = params.pos,
        .prev_pos = params.pos,
        .scale = params.scale,
        .rotation = params.rotation,
    };

    Render2DComponent render2d{
        .color = params.color,
        .size = text_size,
        .layer = params.layer,
        .material_id = params.material_id,
        .render_pass = params.render_pass,
        .is_visible = params.is_visible,
    };

    TextComponent text{
        .font = font,
        .glyph_size = glyph_size,
        .spacing = params.spacing,
        .offset = offset,
        .length = length,
    };

    registry.addComponents(entity, render2d, trans, text);
    return entity;
}

} // namespace lili
