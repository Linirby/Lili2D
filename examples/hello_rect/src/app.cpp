#include "app.hpp"

App::App() {
	window = std::make_unique<lili::Window>("hello_sprite - Lili2D", 800, 800);
	window->set_resizable(true);
	renderer = std::make_unique<lili::Renderer>(window.get());

	rect = std::make_unique<lili::Rect>(
		renderer.get(),
		lili::RectShape(
			window->get_width() / 2.0f - 50.0f,
			window->get_height() / 2.0f - 25.0f,
			100.0f, 50.0f
		),
		lili::Vec4(1.0f, 0.0f, 0.0f, 1.0f)
	);

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

	rect->set_position({
		window->get_width() / 2.0f - rect->get_size().x / 2.0f,
		window->get_height() / 2.0f - rect->get_size().y / 2.0f
	});
	rect->draw();

	renderer->end_frame();
}
