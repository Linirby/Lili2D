#pragma once

#include <memory>
#include <lili2d/lili2d.hpp>

class App {
public:
	App();

	void run();

private:
	std::unique_ptr<lili::Window> window;
	std::unique_ptr<lili::Renderer> renderer;

	lili::Rect cursor_rect;
	lili::Circle cursor_circle;
	lili::Line cursor_line;
	lili::Vec4 color;
	lili::Rect random_rect;

	bool draw_rect;
	bool draw_circle;
	bool draw_line;

	bool running;

	void handleEvents();
	void update();
	void render();
};
