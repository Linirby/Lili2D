#include "lili2d/entities/line.hpp"
#include "lili2d/core/transform.hpp"
#include "lili2d/ecs/entity.hpp"
#include "lili2d/geometry/vec2.hpp"
#include "lili2d/render/2d/line.hpp"
#include "lili2d/render/2d/render2d_component.hpp"

namespace lili {

[[nodiscard]] Entity
createLine(
    ECSRegistry& registry,
    Vec2 start_pos,
    Vec2 end_pos,
    float thickness,
    Vec4 color,
    float layer,
    RenderLayer render_pass,
    bool is_visible,
    uint16_t material_id
)
{
    Entity entity = registry.createEntity();

    TransformComponent transform{ .pos = start_pos,
                                  .prev_pos = start_pos,
                                  .scale = { 1.0f, 1.0f },
                                  .rotation = 0.0f };
    registry.emplaceComponent<TransformComponent>(entity, transform);

    Render2DComponent render2d{ .layer = layer,
                                .render_pass = render_pass,
                                .is_visible = is_visible };
    registry.emplaceComponent<Render2DComponent>(entity, render2d);

    LineComponent line{ .color = color,
                        .dxy = end_pos - start_pos,
                        .thickness = thickness,
                        .material_id = material_id };
    registry.emplaceComponent<LineComponent>(entity, line);
    return entity;
}

} // namespace lili
