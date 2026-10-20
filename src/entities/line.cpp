#include "lili2d/entities/line.hpp"
#include "lili2d/components/line.hpp"
#include "lili2d/components/render2d.hpp"
#include "lili2d/components/transform.hpp"
#include "lili2d/ecs/entity.hpp"
#include "lili2d/geometry/vec2.hpp"
#include <cstdlib>

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

    Render2DComponent render2d{ .color = color,
                                .size = { std::abs(end_pos.x - start_pos.x),
                                          std::abs(end_pos.y - start_pos.y) },
                                .layer = layer,
                                .material_id = material_id,
                                .render_pass = render_pass,
                                .is_visible = is_visible };
    registry.emplaceComponent<Render2DComponent>(entity, render2d);

    LineComponent line{ .dxy = end_pos - start_pos, .thickness = thickness };
    registry.emplaceComponent<LineComponent>(entity, line);
    return entity;
}

} // namespace lili
