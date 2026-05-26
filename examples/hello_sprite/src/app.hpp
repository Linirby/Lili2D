#pragma once

#include <memory>
#include <lili/core.hpp>
#include <lili/render.hpp>

class App {
public:
	App();
	~App() = default;

	void run();

private:
	std::unique_ptr<lili::Window> window;
	std::unique_ptr<lili::Renderer> renderer;
	lili::Sprite cat_sprite;

	bool running;

	void handle_events();
	void render();
};
