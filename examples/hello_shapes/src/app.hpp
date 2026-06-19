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
	lili::Line thin_line;
	lili::Line large_line;
	lili::Rect rect;
	lili::Circle clean_circle;
	lili::Circle low_poly_circle;

	bool running;

	void handleEvents();
	void render();
};
