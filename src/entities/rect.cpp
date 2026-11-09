#include "lili2d/entities/rect.hpp"
#include "lili2d/ecs/entity.hpp"

namespace lili {

[[nodiscard]] Entity
createRect(ECSRegistry& registry, const RectParameters& params)
{
    Entity entity = registry.createEntity();

    TransformComponent transform = params.transform;
    transform.prev_pos = params.transform.pos;

    registry.addComponents(entity, transform, params.render2d, params.rect);
    return entity;
}

} // namespace lili
