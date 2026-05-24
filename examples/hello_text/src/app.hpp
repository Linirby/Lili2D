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

	std::unique_ptr<lili::BitmapFont> font_example;
	float letter_spacing;
	std::unique_ptr<lili::Text> welcome_text;
	std::unique_ptr<lili::Text> info_text;

	bool running;

	void handle_events();
	void render();
};
