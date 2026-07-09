#include "app.hpp"

App::App() : lili::Game("hello_sprite - Lili2D", 800, 800) {
	window->setResizable(true);

	cat_sprite = lili::Sprite(renderer.get(), "cat.png");
	cat_sprite.setScale({ 0.5f, 0.5f });
	cat_sprite.setPosition({ 400, 50 });
	cat_sprite.setRotation(45.0f);
}

void App::onRender(float alpha) {
	(void)alpha;
	cat_sprite.draw();
}
