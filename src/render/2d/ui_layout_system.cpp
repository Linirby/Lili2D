#include "lili2d/render/2d/ui_layout_system.hpp"
#include "lili2d/core/transform.hpp"
#include "lili2d/ecs/ecs_view.hpp"
#include "lili2d/geometry/vec2.hpp"
#include "lili2d/render/2d/render2d_component.hpp"
#include "lili2d/render/2d/ui_layout.hpp"

namespace lili {

void
UILayoutSystem::update(ECSRegistry& registry, Vec2 viewport_size)
{
    auto view =
        registry
            .view<UILayoutComponent, Render2DComponent, TransformComponent>();
    for (auto&& [entity, layout, render2d, trans] : view) {
        Vec2 obj_size = (layout.size != Vec2{ 0.0f, 0.0f })
                            ? layout.size
                            : Vec2{ render2d.size.x * trans.scale.x,
                                    render2d.size.y * trans.scale.y };
        Vec2 topleft = getScreenPosition(layout, obj_size, viewport_size);
        trans.pos = topleft + obj_size * 0.5f;
    }
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
