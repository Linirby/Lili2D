#include "app.hpp"
#include <lili2d/components/ui_layout.hpp>
#include <lili2d/core/action_map.hpp>
#include <lili2d/entities/circle.hpp>
#include <lili2d/entities/line.hpp>
#include <lili2d/entities/rect.hpp>
#include <lili2d/entities/text.hpp>

using namespace lili;

App::App()
  : Game("hello_collision - Lili2D", 800, 800)
{
    ECSRegistry& registry = getECSRegistry();

    cursor_rect = createRect(
        registry,
        {
            .render2d{
                .color = Vec4(0.0f, 0.0f, 1.0f, 1.0f),
                .size = Vec2(75.0f, 75.0f),
            },
        }
    );
    registry.addComponents(cursor_rect, CursorComponent{ ShapeMode::RECT });

    cursor_circle = createCircle(
        registry,
        {
            .render2d{
                .color = Vec4(0.0f, 0.0f, 1.0f, 1.0f),
            },
            .circle{
                .radius = 30.0f,
            },
        }
    );
    registry.addComponents(cursor_circle, CursorComponent{ ShapeMode::CIRCLE });

    cursor_line = createLine(
        registry,
        {
            .render2d{
                .color = Vec4(0.0f, 0.0f, 1.0f, 1.0f),
            },
            .line{
                .thickness = 3.0f,
            },
        }
    );
    registry.addComponents(cursor_line, CursorComponent{ ShapeMode::LINE });

    random_rect = createRect(
        registry,
        {
            .transform{
                .pos = Vec2(400.0f, 400.0f),
            },
            .render2d{
                .color = Vec4(1.0f, 0.0f, 0.0f, 1.0f),
                .size = Vec2(100.0f, 100.0f),
                .layer = -1.0f,
            },
        }
    );

    line_debug_box = createRect(
        registry,
        {
            .render2d{
                .color = Vec4(0.0f, 1.0f, 0.0f, 1.0f),
                .is_visible = false,
            },
            .rect{
                .hollow = true,
            },
        }
    );

    instruction_text = createText(
        registry,
        "KEYS:\n"
        "- rect: 1\n"
        "- circle: 2\n"
        "- line: 3",
        {
            .transform{ .scale = { 3.0f, 3.0f } },
            .render2d{ .render_pass = lili::RenderLayer::UI },
        }
    );
    registry.addComponents(
        instruction_text,
        cUILayout{
            .offset = { -10.0f, -10.0f },
            .anchor = lili::Anchor::BOTTOM_RIGHT,
            .pivot = lili::Pivot::BOTTOM_RIGHT,
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
