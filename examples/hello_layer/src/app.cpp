#include "app.hpp"

#include <string>

App::App() : lili::Game("hello_layer - Lili2D", 1024, 576) {
    lili::Renderer* renderer = getRenderer();
    lili::Window* window = getWindow();

    lili::Texture* tex1 = lili::Assets::loadTexture(
        "layer_1", "layer_1.png", renderer->getDevice()
    );
    lili::Texture* tex2 = lili::Assets::loadTexture(
        "layer_2", "layer_2.png", renderer->getDevice()
    );
    lili::Texture* tex3 = lili::Assets::loadTexture(
        "layer_3", "layer_3.png", renderer->getDevice()
    );

    layer_1 = lili::Sprite(renderer, tex1);
    layer_1.setLayer(1.0f);
    layer_1.setSize(window->getSize());

    layer_2 = lili::Sprite(renderer, tex2);
    layer_2.setLayer(0.0f);
    layer_2.setSize(window->getSize());

    layer_3 = lili::Sprite(renderer, tex3);
    layer_3.setLayer(-1.0f);
    layer_3.setSize(window->getSize());

    red_square = lili::Rect(
        renderer, lili::RectShape(100.0f, 350.0f, 64.0f, 96.0f),
        lili::Vec4(1.0f, 0.0f, 0.0f, 1.0f)
    );
    red_square_layer = 1;
    red_square.setLayer(static_cast<float>(red_square_layer));

    lili::BitmapFont* font =
        lili::Assets::loadFont("lili_font", renderer, "lili_font.png", 16, 6);
    text_current_layer = lili::Text(renderer, font, "Current layer: 1");
    text_current_layer.setScale(2.0f);
    text_current_layer.setRender(lili::RenderLayer::UI);
    text_current_layer.setAnchor(lili::Anchor::BOTTOM_LEFT);
    text_current_layer.setPivot(lili::Pivot::BOTTOM_LEFT);
    text_current_layer.setOffset({10.0f, -32.0f});

    text_control_info =
        lili::Text(renderer, font, "I/K: Increase/decrease red rect layer");
    text_control_info.setScale(2.0f);
    text_control_info.setRender(lili::RenderLayer::UI);
    text_control_info.setAnchor(lili::Anchor::BOTTOM_LEFT);
    text_control_info.setPivot(lili::Pivot::BOTTOM_LEFT);
    text_control_info.setOffset({10.0f, -10.0f});
}

void
App::onEvent(const lili::Event& event) {
    lili::Game::onEvent(event);
    if (event.type() == lili::EventType::KEYBOARD) {
        lili::KeyboardEvent kb = event.keyboard();
        if (kb.action == lili::KeyAction::PRESSED && !kb.repeat) {
            if (kb.key == SDLK_ESCAPE) {
                shutdown();
            } else if (kb.scancode == SDL_SCANCODE_I) {
                red_square_layer += 1;
                red_square.setLayer(static_cast<float>(red_square_layer));
                text_current_layer.setText(
                    "Current layer: " + std::to_string(red_square_layer)
                );
            } else if (kb.scancode == SDL_SCANCODE_K) {
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
App::onRender(float alpha) {
    (void)alpha;
    red_square.draw();

    layer_1.draw();
    layer_2.draw();
    layer_3.draw();

    text_current_layer.draw();
    text_control_info.draw();
}
