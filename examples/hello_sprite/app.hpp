#pragma once

#include <memory>
#include <lili/core.hpp>
#include <lili/render.hpp>

class HelloSprite {
public:
	HelloSprite();
	~HelloSprite() = default;

	void run();

private:
	std::unique_ptr<lili::Window> window;
	std::unique_ptr<lili::Renderer> renderer;

	std::unique_ptr<lili::Sprite> sprite;

	bool running;

	void init();
	void handle_events();
	void render();
	void mainloop();
};
