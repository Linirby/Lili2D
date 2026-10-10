#include "lili2d/entities/rect.hpp"
#include "lili2d/core/transform.hpp"
#include "lili2d/ecs/entity.hpp"
#include "lili2d/render/2d/rect.hpp"
#include "lili2d/render/2d/render2d_component.hpp"
#include "lili2d/render/2d/sprite.hpp"

namespace lili {

[[nodiscard]] Entity
createRect(
    ECSRegistry& registry,
    Vec2 pos,
    Vec2 size,
    Vec2 scale,
    float rotation,
    Vec4 color,
    bool hollow,
    float thickness,
    float layer,
    RenderLayer render_pass,
    bool is_visible,
    uint16_t material_id
)
{
    Entity entity = registry.createEntity();

    TransformComponent transform{
        .pos = pos, .prev_pos = pos, .scale = scale, .rotation = rotation
    };
    registry.emplaceComponent<TransformComponent>(entity, transform);

    Render2DComponent render2d{ .layer = layer,
                                .render_pass = render_pass,
                                .is_visible = is_visible };
    registry.emplaceComponent<Render2DComponent>(entity, render2d);

    RectComponent rect{ .color = color,
                        .size = size,
                        .thickness = thickness,
                        .material_id = material_id,
                        .hollow = hollow };
    registry.emplaceComponent<RectComponent>(entity, rect);

    return entity;
}

} // namespace lili
