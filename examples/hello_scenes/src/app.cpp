#include "app.hpp"

#include "scenes/main_menu.hpp"

using namespace lili;

App::App()
  : Game("hello_scenes - Lili2D", 800, 800)
{
    setTps(20.0f);
    getSceneManager()->push(std::make_unique<MainScene>());
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
}

void
App::onUpdate(float dt)
{
    Game::onUpdate(dt);
}
