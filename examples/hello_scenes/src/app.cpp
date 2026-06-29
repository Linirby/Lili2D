#include "app.hpp"
#include "scenes/main_menu.hpp"

App::App() {
	window = std::make_unique<lili::Window>(
		"hello_scenes - Lili2D", 800, 800
	);
	renderer = std::make_unique<lili::Renderer>(window.get());
	clock = lili::Clock(20.0f);

	scene_manager = lili::SceneManager();
	scene_manager.push(std::make_unique<MainScene>(renderer.get()));

	running = true;
}

void App::run() {
	while (running) {
		handleEvents();
		update(clock.getDt());
		render(clock.getAlpha());
	}
}

void App::handleEvents() {
	lili::Event event;

	while (event.poll()) {
		if (event.type() == lili::EventType::QUIT)
			running = false;
		scene_manager.handleEvents(event);
	}
}

void App::update(float dt) {
	if (scene_manager.empty())
		running = false;
	scene_manager.update(dt);
}

void App::render(float alpha) {
	if (!renderer->beginFrame()) return;

	scene_manager.render(alpha);

	renderer->endFrame();
}
