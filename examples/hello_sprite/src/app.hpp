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
	lili::Sprite cat_sprite;

	bool running;

	void handleEvents();
	void render();
};
