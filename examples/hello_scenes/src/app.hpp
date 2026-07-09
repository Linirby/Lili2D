#pragma once

#include <lili2d/lili2d.hpp>

class App : public lili::Game {
public:
	App();

private:
	lili::SceneManager scene_manager;

	void onEvent(const lili::Event &event) override;
	void onUpdate(float dt) override;
	void onRender(float alpha) override;
};
