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
	std::unique_ptr<lili::Rect> rect;

	bool running;

	void handle_events();
	void render();
};
