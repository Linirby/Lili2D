#pragma once

#include <memory>
#include <lili/core.hpp>
#include <lili/render.hpp>

class App {
public:
	App();

	void run();

private:
	std::unique_ptr<lili::Window> window;
	std::unique_ptr<lili::Renderer> renderer;
	lili::Clock clock;

	lili::AtlasMap head_atlas;
	lili::AnimatedSprite head_sprite;

	bool running;

	void handleEvents();
	void update(float dt);
	void render();
};
