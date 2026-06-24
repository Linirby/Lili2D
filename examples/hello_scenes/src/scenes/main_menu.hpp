#pragma once

#include <lili2d/lili2d.hpp>

class AnotherScene;
class YetAgainScene;

class MainScene: public lili::Scene {
public:
	MainScene(lili::Renderer *renderer);

	void handle_events(const lili::Event &event) override;
	void render(lili::Renderer *renderer, float alpha) override;

private:
	lili::Renderer *renderer;
	lili::BitmapFont font;
	lili::Text text;
	lili::Text info_text;
};
