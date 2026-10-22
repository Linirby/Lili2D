#include "app.hpp"

#include <lili2d/components/render2d.hpp>
#include <lili2d/ecs/ecs_registry.hpp>
#include <lili2d/entities/rect.hpp>
#include <lili2d/entities/sprite.hpp>
#include <lili2d/geometry/vec2.hpp>
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
            .color = Vec4(1.0f, 0.0f, 0.0f, 1.0f),
            .pos = Vec2(100.0f, 400.0f),
            .size = Vec2(64.0f, 96.0f),
            .layer = static_cast<float>(red_square_layer),
        }
    );

    Vec2 window_size = window->getSize();
    layer_1 = createSprite(
        registry,
        renderer,
        "layer_1.png",
        { .pos = window_size * 0.5, .layer = 1.0f }
    );
    registry.getComponent<Render2DComponent>(layer_1).size = window->getSize();

    layer_2 = createSprite(
        registry,
        renderer,
        "layer_2.png",
        { .pos = window_size * 0.5, .layer = 0.0f }
    );
    registry.getComponent<Render2DComponent>(layer_2).size = window->getSize();

    layer_3 = createSprite(
        registry,
        renderer,
        "layer_3.png",
        { .pos = window_size * 0.5, .layer = -1.0f }
    );
    registry.getComponent<Render2DComponent>(layer_3).size = window->getSize();

    BitmapFont* font =
        Assets::loadFont("lili_font", renderer, "lili_font.png", 16, 6);
    text_current_layer = Text(renderer, font, "Current layer: 1");
    text_current_layer.setScale(2.0f);
    text_current_layer.setRender(RenderLayer::UI);
    text_current_layer.setAnchor(Anchor::BOTTOM_LEFT);
    text_current_layer.setPivot(Pivot::BOTTOM_LEFT);
    text_current_layer.setOffset({ 10.0f, -32.0f });

    text_control_info =
        Text(renderer, font, "I/K: Increase/decrease red rect layer");
    text_control_info.setScale(2.0f);
    text_control_info.setRender(RenderLayer::UI);
    text_control_info.setAnchor(Anchor::BOTTOM_LEFT);
    text_control_info.setPivot(Pivot::BOTTOM_LEFT);
    text_control_info.setOffset({ 10.0f, -10.0f });
}

void
App::onEvent(const Event& event)
{
    Game::onEvent(event);

    ECSRegistry& registry = getECSRegistry();

    if (event.type() == EventType::KEYBOARD) {
        KeyboardEvent kb = event.keyboard();
        if (kb.action == KeyAction::PRESSED && !kb.repeat) {
            if (kb.key == Key::ESCAPE)
                shutdown();
            else if (kb.key == Key::I) {
                red_square_layer += 1;
                registry.getComponent<Render2DComponent>(red_square).layer =
                    red_square_layer;

                text_current_layer.setText(
                    "Current layer: " + std::to_string(red_square_layer)
                );
            } else if (kb.key == Key::K) {
                red_square_layer -= 1;
                registry.getComponent<Render2DComponent>(red_square).layer =
                    red_square_layer;

                text_current_layer.setText(
                    "Current layer: " + std::to_string(red_square_layer)
                );
            }
        }
    }
}

void
App::onRender(float alpha)
{
    Game::onRender(alpha);

    text_current_layer.draw();
    text_control_info.draw();
}
