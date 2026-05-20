#pragma once

#include <memory>
#include "lili/core.hpp"

class HelloSprite {
public:
	HelloSprite();
	~HelloSprite() = default;

	void run();

private:
	std::unique_ptr<lili::Window> window;
	bool running;

	void init();
	void handle_events();
	void render();
	void mainloop();
};

HelloSprite::HelloSprite() {
	window = nullptr;
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
	
}

void HelloSprite::mainloop() {
	while (running) {
		handle_events();
		render();
	}
}
