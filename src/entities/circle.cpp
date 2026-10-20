#include "lili2d/entities/circle.hpp"
#include "lili2d/components/circle.hpp"
#include "lili2d/components/render2d.hpp"
#include "lili2d/components/transform.hpp"
#include "lili2d/ecs/entity.hpp"
#include "lili2d/geometry/vec2.hpp"
#include <cstdint>

namespace lili {

[[nodiscard]] Entity
createCircle(
    ECSRegistry& registry,
    Vec2 pos,
    float radius,
    uint32_t segments,
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
        .pos = pos, .prev_pos = pos, .scale = { 1.0f, 1.0f }, .rotation = 0.0f
    };
    registry.emplaceComponent<TransformComponent>(entity, transform);

    Render2DComponent render2d{ .color = color,
                                .size = { radius * 2.0f, radius * 2.0f },
                                .layer = layer,
                                .material_id = material_id,
                                .render_pass = render_pass,
                                .is_visible = is_visible };
    registry.emplaceComponent<Render2DComponent>(entity, render2d);

    CircleComponent circle{ .radius = radius,
                            .segments = segments,
                            .thickness = thickness,
                            .hollow = hollow };
    registry.emplaceComponent<CircleComponent>(entity, circle);
    return entity;
}

} // namespace lili
