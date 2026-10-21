#pragma once

#include <lili2d/lili2d.hpp>

enum class ShapeMode : uint8_t
{
    RECT,
    CIRCLE,
    LINE
};

struct CursorComponent
{
    ShapeMode mode = ShapeMode::RECT;
};

class CursorSystem
  : public lili::System<
        CursorComponent,
        lili::TransformComponent,
        lili::Render2DComponent>
{
public:
    static void
    update(
        lili::ECSRegistry& registry,
        float dt,
        lili::ThreadPool* thread_pool,
        ShapeMode& active_shape,
        lili::Entity target_rect,
        lili::Entity debug_box
    );

    CursorSystem(
        ShapeMode active_shape,
        lili::Vec2 mouse_pos,
        lili::AABB2 target_aabb,
        lili::Entity debug_box
    )
      : active_shape(active_shape)
      , mouse_pos(mouse_pos)
      , target_aabb(target_aabb)
      , line_debug_box(debug_box)
    {
    }

    void
    updateEntity(
        lili::ECSRegistry& registry,
        lili::Entity entity,
        CursorComponent& cursor,
        lili::TransformComponent& trans,
        lili::Render2DComponent& render2d,
        float dt
    ) override;

private:
    ShapeMode active_shape;
    lili::Vec2 mouse_pos;
    lili::AABB2 target_aabb;
    lili::Entity line_debug_box = 0;
};
