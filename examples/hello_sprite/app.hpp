#pragma once

#include <memory>
#include "lili/core.hpp"
#include "lili/render.hpp"

class HelloSprite {
public:
	HelloSprite();
	~HelloSprite() = default;

	void run();

private:
	std::unique_ptr<lili::Window> window;
	std::unique_ptr<lili::Renderer> renderer;

	bool running;

	void init();
	void handle_events();
	void render();
	void mainloop();
};
