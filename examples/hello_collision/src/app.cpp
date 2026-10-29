#include "app.hpp"

#include <lili2d/core/action_map.hpp>
#include <lili2d/entities/circle.hpp>
#include <lili2d/entities/line.hpp>
#include <lili2d/entities/rect.hpp>
#include <lili2d/geometry/vec2.hpp>
#include <lili2d/geometry/vec4.hpp>

using namespace lili;

App::App()
  : Game("hello_collision - Lili2D", 800, 800)
{
    ECSRegistry& registry = getECSRegistry();

    cursor_rect = createRect(
        registry,
        {
            .color = Vec4(0.0f, 0.0f, 1.0f, 1.0f),
            .pos = Vec2(0.0f, 0.0f),
            .size = Vec2(75.0f, 75.0f),
        }
    );
    registry.addComponents(cursor_rect, CursorComponent{ ShapeMode::RECT });

    cursor_circle = createCircle(
        registry,
        {
            .transform{
                .pos = Vec2(0.0f, 0.0f),
            },
            .render2d{
                .color = Vec4(0.0f, 0.0f, 1.0f, 1.0f),
            },
            .circle{
                .radius = 30.0f,
                .segments = 16,
            },
        }
    );
    registry.addComponents(cursor_circle, CursorComponent{ ShapeMode::CIRCLE });

    cursor_line = createLine(
        registry,
        {
            .color = Vec4(0.0f, 0.0f, 1.0f, 1.0f),
            .start_pos = Vec2(0.0f, 0.0f),
            .end_pos = Vec2(0.0f, 0.0f),
            .thickness = 3.0f,
        }
    );
    registry.addComponents(cursor_line, CursorComponent{ ShapeMode::LINE });

    random_rect = createRect(
        registry,
        {
            .color = Vec4(1.0f, 0.0f, 0.0f, 1.0f),
            .pos = Vec2(400.0f, 400.0f),
            .size = Vec2(100.0f, 100.0f),
            .layer = -1.0f,
        }
    );

    line_debug_box = createRect(
        registry,
        {
            .color = Vec4(0.0f, 1.0f, 0.0f, 1.0f),
            .pos = Vec2(0.0f, 0.0f),
            .size = Vec2(0.0f, 0.0f),
            .thickness = 1.0f,
            .hollow = true,
            .is_visible = false,
        }
    );

    ActionMap& actions = ActionMap::get();
    actions.add("cursor_select:rect", { Key::NUM_1, Key::KP_1 });
    actions.add("cursor_select:circle", { Key::NUM_2, Key::KP_2 });
    actions.add("cursor_select:line", { Key::NUM_3, Key::KP_3 });
}

void
App::onEvent(const Event& event)
{
    Game::onEvent(event);

    if (event.type() == EventType::KEYBOARD) {
        KeyboardEvent kb = event.keyboard();
        if (event.keyJustPressed(kb)) {
            if (kb.key == Key::ESCAPE)
                shutdown();
            else if (kb.key == Key::NUM_1 || kb.key == Key::KP_1)
                active_shape = ShapeMode::RECT;
            else if (kb.key == Key::NUM_2 || kb.key == Key::KP_2)
                active_shape = ShapeMode::CIRCLE;
            else if (kb.key == Key::NUM_3 || kb.key == Key::KP_3)
                active_shape = ShapeMode::LINE;
        }
    }
}

void
App::onUpdate(float dt)
{
    Game::onUpdate(dt);

    ECSRegistry& registry = getECSRegistry();
    CursorSystem::update(
        registry, dt, getThreadPool(), active_shape, random_rect, line_debug_box
    );
}
