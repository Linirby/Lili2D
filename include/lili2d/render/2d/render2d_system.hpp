#pragma once

#include "lili2d/ecs/ecs_registry.hpp"
#include "lili2d/render/2d/sprite_batch.hpp"

namespace lili {

class Render2DSystem
{
public:
    static void
    render(ECSRegistry& registry, SpriteBatch& batch)
    {
        renderSprites(registry, batch);
    }

private:
    static void
    renderSprites(ECSRegistry& registry, SpriteBatch& batch);
};

} // namespace lili
