#include "app.hpp"

#include <lili2d/components/render2d.hpp>
#include <lili2d/components/text.hpp>
#include <lili2d/components/ui_layout.hpp>
#include <lili2d/core/string_arena.hpp>
#include <lili2d/ecs/ecs_registry.hpp>
#include <lili2d/entities/rect.hpp>
#include <lili2d/entities/sprite.hpp>
#include <lili2d/geometry/vec2.hpp>
#include <lili2d/render/gpu/pass_types.hpp>
#include <string>

using namespace lili;

App::App()
  : Game("hello_layer - Lili2D", 1024, 576)
{
    ECSRegistry& registry = getECSRegistry();
    Renderer* renderer = getRenderer();
    Window* window = getWindow();

    red_square_layer = 1;
    red_square = createRect(
        registry,
        {
            .transform{
                .pos = Vec2(100.0f, 400.0f),
            },
            .render2d{
                .color = Vec4(1.0f, 0.0f, 0.0f, 1.0f),
                .size = Vec2(64.0f, 96.0f),
                .layer = static_cast<float>(red_square_layer),
            },
        }
    );

    Vec2 window_size = window->getSize();
    layer_1 = createSprite(
        registry,
        renderer,
        "layer_1.png",
        {
            .transform{
                .pos = window_size * 0.5,
            },
            .render2d{
                .layer = 1.0f,
            },
        }
    );
    registry.getComponent<Render2DComponent>(layer_1).size = window->getSize();

    layer_2 = createSprite(
        registry,
        renderer,
        "layer_2.png",
        {
            .transform{
                .pos = window_size * 0.5,
            },
            .render2d{
                .layer = 0.0f,
            },
        }
    );
    registry.getComponent<Render2DComponent>(layer_2).size = window->getSize();

    layer_3 = createSprite(
        registry,
        renderer,
        "layer_3.png",
        {
            .transform{
                .pos = window_size * 0.5,
            },
            .render2d{
                .layer = -1.0f,
            },
        }
    );
    registry.getComponent<Render2DComponent>(layer_3).size = window->getSize();

    text_current_layer = createText(
        registry,
        std::string(MAX_LAYER_TEXT_LENGTH, ' '),
        {
            .transform{ .scale = { 2.0f, 2.0f } },
            .render2d{ .render_pass = lili::RenderLayer::UI },
        }
    );
    registry.addComponents(
        text_current_layer,
        UILayoutComponent{
            .offset = { 10.0f, -32.0f },
            .anchor = lili::Anchor::BOTTOM_LEFT,
            .pivot = lili::Pivot::BOTTOM_LEFT,
        }
    );

    text_control_info = createText(
        registry,
        "I/K: Increase/decrease red rect layer",
        {
            .transform{ .scale = { 2.0f, 2.0f } },
            .render2d{ .render_pass = lili::RenderLayer::UI },
        }
    );
    registry.addComponents(
        text_control_info,
        UILayoutComponent{
            .offset = { 10.0f, -10.0f },
            .anchor = lili::Anchor::BOTTOM_LEFT,
            .pivot = lili::Pivot::BOTTOM_LEFT,
        }
    );
}

void
App::onEvent(const Event& event)
{
    Game::onEvent(event);

    ECSRegistry& registry = getECSRegistry();
    StringArena& string_arena = registry.getStringArena();

    if (event.type() == EventType::KEYBOARD) {
        KeyboardEvent kb = event.keyboard();
        if (kb.action == KeyAction::PRESSED && !kb.repeat) {
            if (kb.key == Key::ESCAPE)
                shutdown();
            else if (kb.key == Key::I) {
                red_square_layer += 1;
                registry.getComponent<Render2DComponent>(red_square).layer =
                    red_square_layer;

                TextComponent& text_comp =
                    registry.getComponent<TextComponent>(text_current_layer);
                string_arena.formatAt(
                    text_comp,
                    MAX_LAYER_TEXT_LENGTH,
                    "Current layer: {}",
                    red_square_layer
                );
            } else if (kb.key == Key::K) {
                red_square_layer -= 1;
                registry.getComponent<Render2DComponent>(red_square).layer =
                    red_square_layer;

                TextComponent& text_comp =
                    registry.getComponent<TextComponent>(text_current_layer);
                string_arena.formatAt(
                    text_comp,
                    MAX_LAYER_TEXT_LENGTH,
                    "Current layer: {}",
                    red_square_layer
                );
            }
        }
    }
}
