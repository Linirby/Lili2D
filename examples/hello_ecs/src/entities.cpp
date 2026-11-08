#include "entities.hpp"

#include "components.hpp"
#include <lili2d/entities/sprite.hpp>

using namespace lili;

namespace entities {

Entity
spawnBall(
    ECSRegistry& registry,
    Renderer* renderer,
    Vec2 pos,
    Vec2 vel,
    Vec4 color,
    float scale
)
{
    Entity entity = createSprite(
        registry,
        renderer,
        "circle.png",
        {
            .transform{
                .pos = pos,
                .scale = { scale, scale },
            },
            .render2d{
                .color = color,
            },
        }
    );
    registry.addComponents(entity, VelocityComponent{ .value = vel });
    return entity;
}

} // namespace entities
