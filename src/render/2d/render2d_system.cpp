#include "lili2d/render/2d/render2d_system.hpp"

#include "lili2d/core/transform.hpp"
#include "lili2d/ecs/ecs_view.hpp"
#include "lili2d/render/2d/render2d_component.hpp"
#include "lili2d/render/2d/sprite.hpp"
#include "lili2d/render/2d/sprite_batch.hpp"

namespace lili {

void
Render2DSystem::renderSprites(ECSRegistry& registry, SpriteBatch& batch)
{
    auto view =
        registry.view<Render2DComponent, TransformComponent, SpriteComponent>();

    batch.begin();
    for (auto&& [entity, render2d, trans, sprite] : view) {
        if (!render2d.is_visible)
            continue;
        batch.draw(
            sprite.slice, trans.pos, trans.scale, trans.rotation, sprite.tint
        );
    }
    batch.end();
    batch.draw();
}

} // namespace lili
