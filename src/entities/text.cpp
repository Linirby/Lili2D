#include "lili2d/entities/text.hpp"
#include "lili2d/components/render2d.hpp"
#include "lili2d/components/text.hpp"
#include "lili2d/ecs/ecs_registry.hpp"
#include "lili2d/ecs/entity.hpp"
#include "lili2d/render/2d/bitmap_font.hpp"
#include <string_view>

namespace lili {

Entity
createText(
    ECSRegistry& registry,
    std::string_view content,
    const TextParameters& params
)
{
    Entity entity = registry.createEntity();

    cTransform transform = params.transform;
    transform.prev_pos = params.transform.pos;

    cRender2D render2d = params.render2d;
    render2d.size =
        params.text.font
            ? params.text.font->calculateTextSize(content, params.text.spacing)
            : Vec2(0.0f, 0.0f);

    cText text = params.text;
    text.glyph_size = params.text.font ? params.text.font->getGlyphSize()
                                       : Vec2{ 0.0f, 0.0f };
    text.capacity = params.text.capacity;
    if (content.size() > text.capacity)
        text.capacity = content.size();
    registry.getStringArena().append(content, text);

    registry.addComponents(entity, transform, render2d, text);
    return entity;
}

} // namespace lili
