#pragma once

#include <lili2d/lili2d.hpp>

class MainScene;
class AnotherScene;

class YetAgainScene: public lili::Scene {
public:
	YetAgainScene(lili::Renderer *renderer);

	void handle_events(const lili::Event &event) override;
	void render(lili::Renderer *renderer, float alpha) override;

private:
	lili::Renderer *renderer;
	lili::BitmapFont font;
	lili::Text text;
	lili::Text info_text;
};
