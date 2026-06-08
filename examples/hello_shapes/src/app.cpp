#include "app.hpp"

App::App() {
	window = std::make_unique<lili::Window>("hello_shapes - Lili2D", 500, 350);
	window->set_resizable(true);
	renderer = std::make_unique<lili::Renderer>(window.get());

	thin_line = lili::Line(
		renderer.get(),
		lili::LineShape({ 50.0f, 50.0f }, { 100.0f, 300.0f }, 1.0f),
		lili::Vec4(0.0f, 1.0f, 0.0f, 1.0f)
	);
	large_line = lili::Line(
		renderer.get(),
		lili::LineShape({ 100.0f, 50.0f }, { 150.0f, 300.0f }, 5.0f),
		lili::Vec4(0.0f, 1.0f, 0.0f, 1.0f)
	);

	rect = lili::Rect(
		renderer.get(),
		lili::RectShape(250.0f, 200.0f, 100.0f, 50.0f),
		lili::Vec4(1.0f, 0.0f, 0.0f, 1.0f)
	);

	clean_circle = lili::Circle(
		renderer.get(),
		lili::CircleShape({ 225.0f, 100.0f }, 50.0f, 32.0f),
		lili::Vec4(0.0f, 0.0f, 1.0f, 1.0f)
	);
	// Less segments (32.0f -> 8.0f)
	low_poly_circle = lili::Circle(
		renderer.get(),
		lili::CircleShape({ 375.0f, 100.0f }, 50.0f, 8.0f),
		lili::Vec4(0.0f, 0.0f, 1.0f, 1.0f)
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

	thin_line.draw();
	large_line.draw();

	rect.draw();

	clean_circle.draw();
	low_poly_circle.draw();

	renderer->end_frame();
}
