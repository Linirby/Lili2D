#include "app.hpp"

App::App() {
	window = std::make_unique<lili::Window>("hello_sprite - Lili2D", 625, 300);
	window->set_resizable(true);
	renderer = std::make_unique<lili::Renderer>(window.get());

	font_example = std::make_unique<lili::BitmapFont>(
		renderer.get(), "lili_font.png", 16, 6
	);
	welcome_text = std::make_unique<lili::Text>(
		renderer.get(), font_example.get(), "Welcome to Lili2D"
	);
	welcome_text->set_scale(3.0f);
	letter_spacing = 1.0f;
	welcome_text->set_spacing(letter_spacing);
	welcome_text->set_position({ 150.0f, 150.0f });
	info_text = std::make_unique<lili::Text>(
		renderer.get(), font_example.get(), "<- & ->: change letter spacing"
	);
	info_text->set_scale(3.0f);
	info_text->set_position({ 10.0f, 10.0f });

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
	lili::Keyboard keyboard;

	while (event.poll())
		if (event.type() == lili::EventType::QUIT)
			running = false;

	keyboard.update();
	if (keyboard.pressed(SDL_SCANCODE_ESCAPE))
		running = false;
	if (keyboard.held(SDL_SCANCODE_LEFT))
		letter_spacing -= 0.01f;
	if (keyboard.held(SDL_SCANCODE_RIGHT))
		letter_spacing += 0.01f;
	welcome_text->set_spacing(letter_spacing);
}

void App::render() {
	if (!renderer->begin_frame()) return;

	welcome_text->draw();
	info_text->draw();

	renderer->end_frame();
}
