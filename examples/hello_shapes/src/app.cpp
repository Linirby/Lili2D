#include "app.hpp"
#include <lili2d/components/circle.hpp>
#include <lili2d/components/rect.hpp>
#include <lili2d/core/game.hpp>
#include <lili2d/ecs/ecs_registry.hpp>
#include <lili2d/entities/circle.hpp>
#include <lili2d/entities/line.hpp>
#include <lili2d/entities/rect.hpp>
#include <lili2d/geometry/vec2.hpp>
#include <lili2d/geometry/vec4.hpp>

using namespace lili;

App::App()
  : Game("hello_shapes - Lili2D", 500, 350)
{
    ECSRegistry& registry = getECSRegistry();

    thin_line = createLine(
        registry,
        {
            .color = Vec4(0.0f, 1.0f, 0.0f, 1.0f),
            .start_pos = Vec2(50.0f, 50.0f),
            .end_pos = Vec2(100.0f, 300.0f),
            .thickness = 1.0f,
        }
    );
    large_line = createLine(
        registry,
        {
            .color = Vec4(0.0f, 1.0f, 0.0f, 1.0f),
            .start_pos = { 100.0f, 50.0f },
            .end_pos = { 150.0f, 300.0f },
            .thickness = 5.0f,
        }
    );

    rect = createRect(
        registry,
        {
            .color = Vec4(1.0f, 0.0f, 0.0f, 1.0f),
            .pos = Vec2(325.0f, 225.0f),
            .size = Vec2(100.0f, 50.0f),
            .thickness = 2.0f,
        }
    );

    clean_circle = createCircle(
        registry,
        {
            .transform{
                .pos = Vec2(250.0f, 250.0f),
            },
            .render2d{
                .color = Vec4(0.0f, 0.0f, 1.0f, 1.0f),
            },
            .circle{
                .radius = 50.0f,
                .segments = 24,
                .thickness = 2.0f,
            },
        }
    );
    low_poly_circle = createCircle(
        registry,
        {
            .transform{
                .pos = Vec2(400.0f, 125.0f),
            },
            .render2d{
                .color = Vec4(0.0f, 0.0f, 1.0f, 1.0f),
            },
            .circle{
                .radius = 50.0f,
                .segments = 8,
                .thickness = 2.0f,
            },
        }
    );

    hollow_active = false;
}

void
App::toggleHollow()
{
    hollow_active = !hollow_active;

    ECSRegistry& reg = getECSRegistry();
    reg.getComponent<RectComponent>(rect).hollow = hollow_active;
    reg.getComponent<CircleComponent>(clean_circle).hollow = hollow_active;
    reg.getComponent<CircleComponent>(low_poly_circle).hollow = hollow_active;
}

void
App::onEvent(const Event& event)
{
    Game::onEvent(event);

    if (event.type() == EventType::KEYBOARD) {
        KeyboardEvent kb = event.keyboard();
        if (kb.action == KeyAction::PRESSED && !kb.repeat) {
            if (kb.key == Key::ESCAPE)
                shutdown();
            else if (kb.key == Key::H || kb.key == Key::SPACE)
                toggleHollow();
        }
    }
}
