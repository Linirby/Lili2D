#pragma once

#include <memory>
#include <lili/lili2d.hpp>

class App {
public:
	App();
	~App() = default;

	void run();

private:
	std::unique_ptr<lili::Window> window;
	std::unique_ptr<lili::Renderer> renderer;
	lili::Line line;
	lili::Rect rect;
	lili::Circle circle;

	bool running;

	void handle_events();
	void render();
};
