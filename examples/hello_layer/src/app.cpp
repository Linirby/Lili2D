#include "app.hpp"

#include <string>

#include "lili2d/render/ui/ui_layout.hpp"

App::App() : lili::Game("hello_layer - Lili2D", 1024, 576) {
    lili::Renderer* renderer = getRenderer();

    lili::Texture* tex1 = lili::Assets::loadTexture(
        "layer_1", "layer_1.png", renderer->getDevice()
    );
    lili::Texture* tex2 = lili::Assets::loadTexture(
        "layer_2", "layer_2.png", renderer->getDevice()
    );
    lili::Texture* tex3 = lili::Assets::loadTexture(
        "layer_3", "layer_3.png", renderer->getDevice()
    );

    layer_1 = lili::Sprite(
        renderer, lili::SliceUV{
                      tex1, 0.0f, 0.0f, 1.0f, 1.0f, (float)tex1->getWidth(),
                      (float)tex1->getHeight()
                  }
    );
    layer_1.setLayer(1);
    layer_2 = lili::Sprite(
        renderer, lili::SliceUV{
                      tex2, 0.0f, 0.0f, 1.0f, 1.0f, (float)tex2->getWidth(),
                      (float)tex2->getHeight()
                  }
    );
    layer_2.setLayer(0);
    layer_3 = lili::Sprite(
        renderer, lili::SliceUV{
                      tex3, 0.0f, 0.0f, 1.0f, 1.0f, (float)tex3->getWidth(),
                      (float)tex3->getHeight()
                  }
    );
    layer_3.setLayer(-1);

    red_square = lili::Rect(
        renderer, lili::RectShape(100, 350, 64, 96), lili::Vec4(1, 0, 0, 1)
    );
    red_square_layer = 1;
    red_square.setLayer(red_square_layer);

    lili::BitmapFont* font =
        lili::Assets::loadFont("lili_font", renderer, "lili_font.png", 16, 6);
    text_current_layer = lili::Text(renderer, font, "Current layer: 1");
    text_current_layer.setScale(2);
    text_current_layer.setRender(lili::RenderLayer::UI);
    text_current_layer.setAnchor(lili::Anchor::BOTTOM_LEFT);
    text_current_layer.setPivot(lili::Pivot::BOTTOM_LEFT);
    text_current_layer.setOffset({10.0f, -32.0f});
    text_control_info =
        lili::Text(renderer, font, "I/K: Increase/decrease red rect layer");
    text_control_info.setScale(2);
    text_control_info.setRender(lili::RenderLayer::UI);
    text_control_info.setAnchor(lili::Anchor::BOTTOM_LEFT);
    text_control_info.setPivot(lili::Pivot::BOTTOM_LEFT);
    text_control_info.setOffset({10.0f, -10.0f});
}

void
App::onEvent(const lili::Event& event) {
    lili::KeyboardEvent kb = event.keyboard();

    if (event.type() == lili::EventType::KEYBOARD)
        if (kb.action == lili::KeyAction::PRESSED)
            if (kb.key == SDLK_ESCAPE) shutdown();

    lili::KeyboardEvent k_ev = event.keyboard();
    if (k_ev.action == lili::KeyAction::PRESSED && !k_ev.repeat) {
        if (k_ev.scancode == SDL_SCANCODE_I) {
            red_square_layer += 1;
            red_square.setLayer(red_square_layer);
            text_current_layer.setText(
                "Current layer: " + std::to_string(red_square_layer)
            );
        }
        if (k_ev.scancode == SDL_SCANCODE_K) {
            red_square_layer -= 1;
            red_square.setLayer(red_square_layer);
            text_current_layer.setText(
                "Current layer: " + std::to_string(red_square_layer)
            );
        }
    }
}

void
App::onRender(float alpha) {
    (void)alpha;
    red_square.draw();

    lili::Window* window = getWindow();
    layer_1.setSize(window->getSize());
    layer_1.draw();
    layer_2.setSize(window->getSize());
    layer_2.draw();
    layer_3.setSize(window->getSize());
    layer_3.draw();

    text_current_layer.draw();
    text_control_info.draw();
}
