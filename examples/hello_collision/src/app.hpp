#pragma once

#include <memory>
#include <lili/lili2d.hpp>

class App {
public:
	App();

	void run();

private:
	std::unique_ptr<lili::Window> window;
	std::unique_ptr<lili::Renderer> renderer;

	lili::Rect cursor_rect;
	lili::Vec4 color;
	lili::Rect random_rect;

	bool running;

	void handleEvents();
	void render();
};
