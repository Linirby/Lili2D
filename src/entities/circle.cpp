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

    TransformComponent transform = params.transform;
    registry.emplaceComponent<TransformComponent>(entity, transform);

    Render2DComponent render2d = params.render2d;
    render2d.size = { params.circle.radius * 2.0f,
                      params.circle.radius * 2.0f };
    registry.emplaceComponent<Render2DComponent>(entity, render2d);

    CircleComponent circle = params.circle;
    registry.emplaceComponent<CircleComponent>(entity, circle);
    return entity;
}

} // namespace lili
