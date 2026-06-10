#include "app.hpp"

App::App() {
	window = std::make_unique<lili::Window>("hello_sprite - Lili2D", 800, 800);
	window->setResizable(true);
	renderer = std::make_unique<lili::Renderer>(window.get());

	cat_sprite = lili::Sprite(renderer.get(), "cat.png");
	cat_sprite.setScale({ 0.5f, 0.5f });
	cat_sprite.setPosition({ 400, 50 });
	cat_sprite.setRotation(45.0f);

	running = true;
}

void App::run() {
	while (running) {
		handleEvents();
		render();
	}
}

void App::handleEvents() {
	lili::Event event;

	while (event.poll()) {
		lili::KeyboardEvent keyboard = event.keyboard();

		if (event.type() == lili::EventType::QUIT)
			running = false;
		if (keyboard.key == SDLK_ESCAPE)
			running = false;
	}
}

void App::render() {
	if (!renderer->beginFrame()) return;

	cat_sprite.draw();

	renderer->endFrame();
}
