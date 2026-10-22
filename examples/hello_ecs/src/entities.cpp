#include "entities.hpp"

#include "components.hpp"
#include <lili2d/entities/sprite.hpp>
#include <lili2d/render/renderer.hpp>

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
        SpriteParameters{
            .color = color,
            .pos = pos,
            .scale = { scale, scale },
        }
    );
    registry.addComponents(entity, VelocityComponent{ .value = vel });
    return entity;
}

} // namespace entities
