#include "cursor.hpp"

#include <lili2d/components/circle.hpp>
#include <lili2d/components/line.hpp>
#include <lili2d/components/rect.hpp>
#include <lili2d/core/action_map.hpp>
#include <lili2d/physics/circle_collider.hpp>

using namespace lili;

void
CursorSystem::update(
    ECSRegistry& registry,
    float dt,
    ThreadPool* thread_pool,
    ShapeMode& active_shape,
    Entity target_rect,
    Entity debug_box
)
{
    ActionMap& actions = ActionMap::get();
    if (actions.isJustPressed("cursor_select:rect"))
        active_shape = ShapeMode::RECT;
    else if (actions.isJustPressed("cursor_select:circle"))
        active_shape = ShapeMode::CIRCLE;
    else if (actions.isJustPressed("cursor_select:line"))
        active_shape = ShapeMode::LINE;

    Mouse mouse;
    mouse.update();
    Vec2 mouse_pos = mouse.getPos();

    AABB2 target_aabb{};
    if (registry.isValid(target_rect)) {
        const auto& trans =
            registry.getComponent<TransformComponent>(target_rect);
        const auto& render2d =
            registry.getComponent<Render2DComponent>(target_rect);
        target_aabb = AABB2(trans.pos - render2d.size * 0.5f, render2d.size);
    }

    if (active_shape != ShapeMode::LINE && registry.isValid(debug_box)) {
        registry.getComponent<Render2DComponent>(debug_box).is_visible = false;
    }

    (void)registry.getPool<CircleComponent>();
    (void)registry.getPool<LineComponent>();
    (void)registry.getPool<RectComponent>();

    CursorSystem sys(active_shape, mouse_pos, target_aabb, debug_box);
    sys.run(registry, dt, thread_pool);
}

void
CursorSystem::updateEntity(
    ECSRegistry& registry,
    Entity entity,
    CursorComponent& cursor,
    TransformComponent& trans,
    Render2DComponent& render2d,
    [[maybe_unused]] float dt
)
{
    if (cursor.mode != active_shape) {
        render2d.is_visible = false;
        return;
    }

    render2d.is_visible = true;

    const Vec4 hit_color(0.0f, 1.0f, 0.0f, 1.0f);
    const Vec4 miss_color(0.0f, 0.0f, 1.0f, 1.0f);
    bool hit = false;

    switch (cursor.mode) {
        case ShapeMode::RECT: {
            trans.pos = mouse_pos;
            AABB2 aabb(trans.pos - render2d.size * 0.5f, render2d.size);
            hit = aabb.intersect(target_aabb);
            break;
        }
        case ShapeMode::CIRCLE: {
            trans.pos = mouse_pos;
            if (registry.hasComponent<CircleComponent>(entity)) {
                const auto& circle =
                    registry.getComponent<CircleComponent>(entity);
                CircleCollider collider(trans.pos, circle.radius);
                hit = collider.intersect(target_aabb);
            }
            break;
        }
        case ShapeMode::LINE: {
            trans.pos = Vec2(0.0f, 0.0f);
            if (registry.hasComponent<LineComponent>(entity)) {
                auto& line = registry.getComponent<LineComponent>(entity);
                line.dxy = mouse_pos;

                LineShape shape(trans.pos, trans.pos + line.dxy);
                AABB2 line_box(shape);
                hit = line_box.intersect(target_aabb);

                if (registry.isValid(line_debug_box)) {
                    auto& box_trans = registry.getComponent<TransformComponent>(
                        line_debug_box
                    );
                    auto& box_render2d =
                        registry.getComponent<Render2DComponent>(
                            line_debug_box
                        );
                    box_trans.pos = (line_box.min + line_box.max) * 0.5f;
                    box_render2d.size = line_box.max - line_box.min;
                    box_render2d.is_visible = true;
                }
            }
            break;
        }
    }

    render2d.color = hit ? hit_color : miss_color;
}
