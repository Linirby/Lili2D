#include "app.hpp"

App::App() : lili::Game("hello_animation - Lili2D", 400, 400) {
    setTps(20.0f);
    getWindow()->setResizable(true);

    lili::Renderer* renderer = getRenderer();
    lili::Assets::setHotReloadEnabled(true);

    lili::AtlasMap* head_atlas = lili::Assets::loadAtlas(
        "head_atlas", renderer, "head_animation.png", 8, 1
    );

    head_sprite = lili::AnimatedSprite(
        renderer, lili::Animation(head_atlas->getSliceUVs(0, 8))
    );
    head_sprite.setScale({15.0f, 15.0f});
    head_sprite.setFrameSpeed(0.2f);
    head_sprite.setRender(lili::RenderLayer::UI);
    head_sprite.setAnchor(lili::Anchor::BOTTOM);
    head_sprite.setPivot(lili::Pivot::BOTTOM);
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
App::onUpdate(float dt) {
    head_sprite.update(dt);
}

void
App::onRender(float alpha) {
    (void)alpha;
    head_sprite.draw();
}
