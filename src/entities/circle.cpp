#include "lili2d/entities/circle.hpp"
#include "lili2d/components/circle.hpp"
#include "lili2d/components/render2d.hpp"
#include "lili2d/components/transform.hpp"
#include "lili2d/ecs/entity.hpp"

namespace lili {

[[nodiscard]] Entity
createCircle(ECSRegistry& registry, const CircleParameters& params)
{
    Entity entity = registry.createEntity();

    TransformComponent transform{ .pos = params.pos,
                                  .prev_pos = params.pos,
                                  .scale = { 1.0f, 1.0f },
                                  .rotation = 0.0f };
    registry.emplaceComponent<TransformComponent>(entity, transform);

    Render2DComponent render2d{ .color = params.color,
                                .size = { params.radius * 2.0f,
                                          params.radius * 2.0f },
                                .layer = params.layer,
                                .material_id = params.material_id,
                                .render_pass = params.render_pass,
                                .is_visible = params.is_visible };
    registry.emplaceComponent<Render2DComponent>(entity, render2d);

    CircleComponent circle{ .radius = params.radius,
                            .segments = params.segments,
                            .thickness = params.thickness,
                            .hollow = params.hollow };
    registry.emplaceComponent<CircleComponent>(entity, circle);
    return entity;
}

} // namespace lili
