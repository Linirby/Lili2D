#include "app.hpp"

#include "scenes/main_menu.hpp"

using namespace lili;

App::App()
  : Game("hello_scenes - Lili2D", 800, 800)
{
    setTps(20.0f);
    scene_manager.push(std::make_unique<MainScene>(getRenderer()));
}

void
App::onEvent(const Event& event)
{
    Game::onEvent(event);
    if (event.type() == EventType::KEYBOARD) {
        KeyboardEvent kb = event.keyboard();
        if (kb.action == KeyAction::PRESSED && kb.key == Key::ESCAPE)
            shutdown();
    }
    scene_manager.handleEvents(event);
}

void
App::onUpdate(float dt)
{
    if (!scene_manager.empty())
        scene_manager.update(dt);
}

void
App::onRender(float alpha)
{
    scene_manager.render(alpha);
}
