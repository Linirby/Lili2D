#include "app.hpp"

App::App() {
	window = std::make_unique<lili::Window>("hello_text - Lili2D", 625, 300);
	window->setResizable(true);
	renderer = std::make_unique<lili::Renderer>(window.get());

	font_example = std::make_unique<lili::BitmapFont>(
		renderer.get(), "lili_font.png", 16, 6
	);

	welcome_text = lili::Text(
		renderer.get(), font_example.get(), "Welcome to Lili2D"
	);
	welcome_text.setScale(3.0f);
	letter_spacing = 1.0f;
	welcome_text.setSpacing(letter_spacing);
	welcome_text.setPosition({ 150.0f, 150.0f });

	info_text = lili::Text(
		renderer.get(), font_example.get(), "<- & ->: change letter spacing"
	);
	info_text.setScale(3.0f);
	info_text.setPosition({ 10.0f, 10.0f });

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
	lili::Keyboard keyboard;

	while (event.poll())
		if (event.type() == lili::EventType::QUIT)
			running = false;

	keyboard.update();
	if (keyboard.pressed(SDL_SCANCODE_ESCAPE))
		running = false;

	// Increase/decrease spacing with arrow key
	if (keyboard.held(SDL_SCANCODE_LEFT))
		letter_spacing -= 0.01f;
	if (keyboard.held(SDL_SCANCODE_RIGHT))
		letter_spacing += 0.01f;
	welcome_text.setSpacing(letter_spacing);
}

void App::render() {
	if (!renderer->beginFrame()) return;

	welcome_text.draw();
	info_text.draw();

	renderer->endFrame();
}
