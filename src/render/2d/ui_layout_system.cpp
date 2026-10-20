#include "lili2d/render/2d/ui_layout_system.hpp"
#include "lili2d/components/circle.hpp"
#include "lili2d/components/line.hpp"
#include "lili2d/components/render2d.hpp"
#include "lili2d/components/transform.hpp"
#include "lili2d/components/ui_layout.hpp"
#include "lili2d/geometry/vec2.hpp"
#include <cmath>

namespace lili {

void
UILayoutSystem::updateEntity(
    ECSRegistry& registry,
    Entity entity,
    UILayoutComponent& layout,
    Render2DComponent& render2d,
    TransformComponent& trans,
    [[maybe_unused]] float dt
)
{
    Vec2 base_size = render2d.size;
    if (registry.hasComponent<CircleComponent>(entity)) {
        float d = registry.getComponent<CircleComponent>(entity).radius * 2.0f;
        base_size = { d, d };
    } else if (registry.hasComponent<LineComponent>(entity)) {
        Vec2 dxy = registry.getComponent<LineComponent>(entity).dxy;
        base_size = { std::abs(dxy.x), std::abs(dxy.y) };
    }

    Vec2 obj_size =
        (layout.size != Vec2{ 0.0f, 0.0f })
            ? layout.size
            : Vec2{ base_size.x * trans.scale.x, base_size.y * trans.scale.y };
    Vec2 topleft = getScreenPosition(layout, obj_size, viewport_size);
    trans.pos = topleft + obj_size * 0.5f;
}

void
UILayoutSystem::update(
    ECSRegistry& registry,
    Vec2 viewport_size,
    ThreadPool* thread_pool
)
{
    (void)registry.getPool<CircleComponent>();
    (void)registry.getPool<LineComponent>();
    UILayoutSystem sys(viewport_size);
    sys.run(registry, 0.0f, thread_pool);
}

Vec2
UILayoutSystem::getScreenPosition(
    UILayoutComponent layout,
    Vec2 obj_size,
    Vec2 viewport_size
)
{
    Vec2 anchor_vec = anchorToVector(layout.anchor, layout.custom_anchor);
    Vec2 pivot_vec = pivotToVector(layout.pivot, layout.custom_pivot);

    Vec2 anchor_point =
        Vec2(anchor_vec.x * viewport_size.x, anchor_vec.y * viewport_size.y);
    Vec2 pivot_point = Vec2(pivot_vec.x * obj_size.x, pivot_vec.y * obj_size.y);

    return anchor_point + layout.offset - pivot_point;
}

} // namespace lili
