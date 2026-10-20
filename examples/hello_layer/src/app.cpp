#include "app.hpp"

#include <string>

using namespace lili;

App::App()
  : Game("hello_layer - Lili2D", 1024, 576)
{
    Renderer* renderer = getRenderer();
    Window* window = getWindow();

    Texture* tex1 =
        Assets::loadTexture("layer_1", "layer_1.png", renderer->getDevice());
    Texture* tex2 =
        Assets::loadTexture("layer_2", "layer_2.png", renderer->getDevice());
    Texture* tex3 =
        Assets::loadTexture("layer_3", "layer_3.png", renderer->getDevice());

    layer_1 = Sprite(renderer, tex1);
    layer_1.setLayer(1.0f);
    layer_1.setSize(window->getSize());

    layer_2 = Sprite(renderer, tex2);
    layer_2.setLayer(0.0f);
    layer_2.setSize(window->getSize());

    layer_3 = Sprite(renderer, tex3);
    layer_3.setLayer(-1.0f);
    layer_3.setSize(window->getSize());

    red_square = Rect(
        renderer,
        RectShape(100.0f, 350.0f, 64.0f, 96.0f),
        Vec4(1.0f, 0.0f, 0.0f, 1.0f)
    );
    red_square_layer = 1;
    red_square.setLayer(static_cast<float>(red_square_layer));

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

    if (event.type() == EventType::KEYBOARD) {
        KeyboardEvent kb = event.keyboard();
        if (kb.action == KeyAction::PRESSED && !kb.repeat) {
            if (kb.key == Key::ESCAPE) {
                shutdown();
            } else if (kb.key == Key::I) {
                red_square_layer += 1;
                red_square.setLayer(static_cast<float>(red_square_layer));
                text_current_layer.setText(
                    "Current layer: " + std::to_string(red_square_layer)
                );
            } else if (kb.key == Key::K) {
                red_square_layer -= 1;
                red_square.setLayer(static_cast<float>(red_square_layer));
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

    red_square.draw();

    layer_1.draw();
    layer_2.draw();
    layer_3.draw();

    text_current_layer.draw();
    text_control_info.draw();
}
