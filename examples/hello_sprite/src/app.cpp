#include "app.hpp"

App::App() : lili::Game("hello_sprite - Lili2D", 800, 800) {
	getWindow()->setResizable(true);

	cat_sprite = lili::Sprite(getRenderer(), "cat.png");
	cat_sprite.setScale({ 0.5f, 0.5f });
	cat_sprite.setPosition({ 400, 50 });
	cat_sprite.setRotation(45.0f);
}

void App::onEvent(const lili::Event &event) {
	lili::KeyboardEvent kb = event.keyboard();

	if (event.type() == lili::EventType::KEYBOARD)
		if (kb.action == lili::KeyAction::PRESSED)
			if (kb.key == SDLK_ESCAPE)
				shutdown();
}

void App::onRender(float alpha) {
	(void)alpha;
	cat_sprite.draw();
}
