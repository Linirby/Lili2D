#pragma once

#include <lili2d/lili2d.hpp>

class MainScene;
class YetAgainScene;

class AnotherScene: public lili::Scene {
public:
	AnotherScene(lili::Renderer *renderer);

	void handleEvents(const lili::Event &event) override;
	void render(float alpha) override;

private:
	lili::BitmapFont font;
	lili::Text text;
	lili::Text info_text;
};
