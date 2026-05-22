#include "app.hpp"

HelloSprite::HelloSprite() {
	window = nullptr;
	renderer = nullptr;
	running = false;
}

void HelloSprite::run() {
	init();
	mainloop();
}

void HelloSprite::init() {
	window = std::make_unique<lili::Window>();
	window->set_title("hello_sprite - Lili2D");
	window->set_size(800, 800);
	window->set_resizable(true);

	renderer = std::make_unique<lili::Renderer>();
	renderer->set_window(window.get());

	sprite = std::make_unique<lili::Sprite>();
	sprite->set_texture(renderer.get(), "hello_sprite/cube.png");
	sprite->set_scale({ 100.0f, 100.0f });

	running = true;
}

void HelloSprite::handle_events() {
	lili::Event event;

	while (event.poll()) {
		lili::KeyboardEvent keyboard = event.keyboard();

		if (event.type() == lili::EventType::QUIT)
			running = false;
		if (keyboard.key == SDLK_ESCAPE)
			running = false;
	}
}

void HelloSprite::render() {
	if (!renderer->begin_frame()) return;

	sprite->draw(renderer.get());

	renderer->end_frame();
}

void HelloSprite::mainloop() {
	while (running) {
		handle_events();
		render();
	}
}
