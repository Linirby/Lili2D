#pragma once

#include <memory>
#include <lili2d/lili2d.hpp>
#include <SDL3/SDL.h>

class App {
public:
	App();
	void run();

private:
	void handleEvents();
	void update(float dt);
	void render();

	std::unique_ptr<lili::Window> window;
	std::unique_ptr<lili::Renderer> renderer;
	lili::Clock clock;
	lili::Camera camera;

	lili::AtlasMap env_atlas;
	lili::AtlasMap char_atlas;

	std::unique_ptr<lili::TileMap> tilemap = nullptr;

	bool running = false;

	lili::BitmapFont font;
	lili::Text text_infos;

	lili::Keyboard keyboard;
};
