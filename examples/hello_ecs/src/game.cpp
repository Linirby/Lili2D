#include "game.hpp"

Game::Game() {
	window = std::make_unique<lili::Window>("hello_camera - Lili2D", 600, 400);
	window->setResizable(true);
	renderer = std::make_unique<lili::Renderer>(window.get());

	clock = lili::Clock(20.0f);
	camera = lili::Camera();
	renderer->setCamera(&camera);

	running = true;
}

void Game::run() {
	while (running) {
		clock.update();

		handleEvents();
		update(clock.getDt());
		render();
	}
}

void Game::handleEvents() {
	lili::Event event;

	while (event.poll())
		if (event.type() == lili::EventType::QUIT)
			running = false;
}

void Game::update(float dt) {}

void Game::render() {
	if (!renderer->beginFrame()) return;
	renderer->endFrame();
}
