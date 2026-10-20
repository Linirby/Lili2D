#include "lili2d/entities/rect.hpp"
#include "lili2d/components/rect.hpp"
#include "lili2d/components/render2d.hpp"
#include "lili2d/components/transform.hpp"
#include "lili2d/ecs/entity.hpp"

namespace lili {

[[nodiscard]] Entity
createRect(ECSRegistry& registry, const RectParameters& params)
{
    Entity entity = registry.createEntity();

    TransformComponent transform{ .pos = params.pos,
                                  .prev_pos = params.pos,
                                  .scale = params.scale,
                                  .rotation = params.rotation };
    registry.emplaceComponent<TransformComponent>(entity, transform);

    Render2DComponent render2d{ .color = params.color,
                                .size = { params.size.x, params.size.y },
                                .layer = params.layer,
                                .material_id = params.material_id,
                                .render_pass = params.render_pass,
                                .is_visible = params.is_visible };
    registry.emplaceComponent<Render2DComponent>(entity, render2d);

    RectComponent rect{ .thickness = params.thickness,
                        .hollow = params.hollow };
    registry.emplaceComponent<RectComponent>(entity, rect);

    return entity;
}

} // namespace lili
