#include "app.hpp"
#include "scenes/main_menu.hpp"

App::App() : lili::Game("hello_scenes - Lili2D", 800, 800) {
	setTps(20.0f);

	scene_manager = lili::SceneManager();
	scene_manager.push(std::make_unique<MainScene>(getRenderer()));
}

void App::onUpdate(float dt) {
	if (scene_manager.empty())
		return;
	scene_manager.update(dt);
}

void App::onRender(float alpha) {
	scene_manager.render(alpha);
}

void App::onEvent(const lili::Event &event) {
	lili::KeyboardEvent kb = event.keyboard();

	if (event.type() == lili::EventType::KEYBOARD)
		if (kb.action == lili::KeyAction::PRESSED)
			if (kb.key == SDLK_ESCAPE)
				shutdown();

	scene_manager.handleEvents(event);
}
