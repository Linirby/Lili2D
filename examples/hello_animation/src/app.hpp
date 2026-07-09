#pragma once

#include <lili2d/lili2d.hpp>

class App : public lili::Game {
public:
	App();

private:
	lili::AtlasMap head_atlas;
	lili::AnimatedSprite head_sprite;

	void onUpdate(float dt) override;
	void onRender(float alpha) override;
};
