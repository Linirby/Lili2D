#pragma once

#include <lili2d/lili2d.hpp>

class Game {
public:
	Game();
	void run();

private:
	std::unique_ptr<lili::Window> window;
	std::unique_ptr<lili::Renderer> renderer;

	lili::Clock clock;
	lili::Camera camera;

	bool running;

	void handleEvents();
	void update(float dt);
	void render();
};
