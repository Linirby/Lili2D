#include "app.hpp"

App::App() : lili::Game("hello_text - Lili2D", 625, 300) {
	window->setResizable(true);

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

	speed_change = 10.0f;
}

void App::onUpdate(float dt) {
	keyboard.update();
	if (keyboard.held(SDL_SCANCODE_LEFT)) {
		letter_spacing -= speed_change * dt;
		welcome_text.setSpacing(letter_spacing);
	}
	if (keyboard.held(SDL_SCANCODE_RIGHT)) {
		letter_spacing += speed_change * dt;
		welcome_text.setSpacing(letter_spacing);
	}
}

void App::onRender(float alpha) {
	(void)alpha;
	welcome_text.draw();
	info_text.draw();
}
