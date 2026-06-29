#pragma once

#include <lili2d/lili2d.hpp>

class MainScene;
class AnotherScene;

class YetAgainScene: public lili::Scene {
public:
	YetAgainScene(lili::Renderer *renderer);

	void handleEvents(const lili::Event &event) override;
	void render(float alpha) override;

private:
	lili::BitmapFont font;
	lili::Text text;
	lili::Text info_text;
};
