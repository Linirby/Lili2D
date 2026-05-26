#include "app.hpp"

App::App() {
	window = std::make_unique<lili::Window>("hello_sprite - Lili2D", 800, 800);
	window->set_resizable(true);
	renderer = std::make_unique<lili::Renderer>(window.get());

	cat_sprite = lili::Sprite(renderer.get(), "cat.png");
	cat_sprite.set_scale({ 0.5f, 0.5f });

	running = true;
}

void App::run() {
	while (running) {
		handle_events();
		render();
	}
}

void App::handle_events() {
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
	if (!renderer->begin_frame()) return;

	cat_sprite.draw();

	renderer->end_frame();
}
