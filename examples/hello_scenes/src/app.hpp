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
	lili::Clock clock;

	lili::SceneManager scene_manager;

	bool running;

	void handleEvents();
	void update(float dt);
	void render(float alpha);
};
