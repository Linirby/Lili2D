#include "app.hpp"

#include "scenes/main_menu.hpp"

App::App() : lili::Game("hello_scenes - Lili2D", 800, 800) {
    setTps(20.0f);
    scene_manager.push(std::make_unique<MainScene>(getRenderer()));
}

void
App::onUpdate(float dt) {
    if (!scene_manager.empty()) scene_manager.update(dt);
}

void
App::onRender(float alpha) {
    scene_manager.render(alpha);
}

void
App::onEvent(const lili::Event& event) {
    lili::Game::onEvent(event);
    if (event.type() == lili::EventType::KEYBOARD) {
        lili::KeyboardEvent kb = event.keyboard();
        if (kb.action == lili::KeyAction::PRESSED && kb.key == lili::Key::ESCAPE)
            shutdown();
    }
    scene_manager.handleEvents(event);
}
