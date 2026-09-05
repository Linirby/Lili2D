#include "app.hpp"

App::App() : lili::Game("hello_sprite - Lili2D", 800, 800) {
    lili::Texture* cat_tex = lili::Assets::loadTexture(
        "cat_texture", "cat.png", getRenderer()->getDevice()
    );
    cat_sprite = lili::Sprite(getRenderer(), cat_tex);
    cat_sprite.setScale({0.5f, 0.5f});
    cat_sprite.setPosition({400.0f, 50.0f});
    cat_sprite.setRotation(45.0f);
}

void
App::onEvent(const lili::Event& event) {
    lili::Game::onEvent(event);
    if (event.type() == lili::EventType::KEYBOARD) {
        lili::KeyboardEvent kb = event.keyboard();
        if (kb.action == lili::KeyAction::PRESSED && kb.key == lili::Key::ESCAPE)
            shutdown();
    }
}

void
App::onRender(float alpha) {
    (void)alpha;
    cat_sprite.draw();
}
