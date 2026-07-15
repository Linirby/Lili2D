#pragma once

#include "lili2d/core/thread_pool.hpp"
#include <memory>
#include <lili2d/lili2d.hpp>
#include <SDL3/SDL.h>



class App : public lili::Game {
public:
	App();

private:
	lili::Camera camera;
	lili::Keyboard keyboard;

	lili::AtlasMap env_atlas;
	lili::AtlasMap char_atlas;

	std::unique_ptr<lili::ThreadPool> thread_pool;
	std::unique_ptr<lili::TileMap> tilemap = nullptr;

	lili::BitmapFont font;
	lili::Text text_infos;

	void onEvent(const lili::Event &event) override;
	void onUpdate(float dt) override;
	void onRender(float alpha) override;
};
