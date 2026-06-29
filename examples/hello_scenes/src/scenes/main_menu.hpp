#pragma once

#include <lili2d/lili2d.hpp>

class AnotherScene;
class YetAgainScene;

class MainScene: public lili::Scene {
public:
	MainScene(lili::Renderer *renderer);

	void handleEvents(const lili::Event &event) override;
	void render(float alpha) override;

private:
	lili::BitmapFont font;
	lili::Text text;
	lili::Text info_text;
};
