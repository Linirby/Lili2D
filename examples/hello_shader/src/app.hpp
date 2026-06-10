#pragma once

#include <memory>
#include <lili/core.hpp>
#include <lili/render.hpp>

struct RectUB {
	float time;
	float amplitude;
	float frequency;
	float speed;
};

struct TextUB {
	float speed;
	float time;
};


class App {
public:
	App();
	void run();

private:
	std::unique_ptr<lili::Window> window;
	std::unique_ptr<lili::Renderer> renderer;
	lili::Clock clock;

	std::unique_ptr<lili::Shader> rect_shader;
	std::unique_ptr<lili::WorldPipeline> rect_pipeline;
	lili::Rect rect;

	std::unique_ptr<lili::Shader> text_shader;
	std::unique_ptr<lili::WorldPipeline> text_pipeline;
	std::unique_ptr<lili::BitmapFont> font;
	lili::Text text;
	lili::Text text_info;

	bool toggle_custom_shaders;

	bool running;

	void handleEvents();
	void render();
};
