#pragma once

#include <memory>
#include <lili2d/lili2d.hpp>

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

class App : public lili::Game {
public:
	App();

private:
	lili::Clock clock;

	std::unique_ptr<lili::Shader> rect_shader;
	std::unique_ptr<lili::MainGraphicsPipeline> rect_pipeline;
	lili::Rect rect;

	std::unique_ptr<lili::Shader> text_shader;
	std::unique_ptr<lili::MainGraphicsPipeline> text_pipeline;
	std::unique_ptr<lili::BitmapFont> font;
	lili::Text text;
	lili::Text text_info;

	bool toggle_custom_shaders;

	void onEvent(const lili::Event &event) override;
	void onUpdate(float dt) override;
	void onRender(float alpha) override;
};
