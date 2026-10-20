#include "lili2d/entities/line.hpp"
#include "lili2d/components/line.hpp"
#include "lili2d/components/render2d.hpp"
#include "lili2d/components/transform.hpp"
#include "lili2d/ecs/entity.hpp"
#include "lili2d/geometry/vec2.hpp"
#include <cstdlib>

namespace lili {

[[nodiscard]] Entity
createLine(ECSRegistry& registry, const LineParameters& params)
{
    Entity entity = registry.createEntity();

    TransformComponent transform{ .pos = params.start_pos,
                                  .prev_pos = params.start_pos,
                                  .scale = { 1.0f, 1.0f },
                                  .rotation = 0.0f };
    registry.emplaceComponent<TransformComponent>(entity, transform);

    Render2DComponent render2d{
        .color = params.color,
        .size = { std::abs(params.end_pos.x - params.start_pos.x),
                  std::abs(params.end_pos.y - params.start_pos.y) },
        .layer = params.layer,
        .material_id = params.material_id,
        .render_pass = params.render_pass,
        .is_visible = params.is_visible
    };
    registry.emplaceComponent<Render2DComponent>(entity, render2d);

    LineComponent line{ .dxy = params.end_pos - params.start_pos,
                        .thickness = params.thickness };
    registry.emplaceComponent<LineComponent>(entity, line);
    return entity;
}

} // namespace lili
