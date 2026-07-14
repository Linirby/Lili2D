#pragma once

#include "lili2d/core/game.hpp"
#include "lili2d/ecs.hpp"
#include "lili2d/render/scene/core2d/sprite_batch.hpp"
#include "lili2d/world/camera.hpp"
#include <memory>
#include <vector>

class App : public lili::Game {
public:
	App();
	~App() override = default;

	void onInit() override;
	void onEvent(const lili::Event &event) override;
	void onUpdate(float dt) override;
	void onRender(float alpha) override;

private:
	void spawnRandomBall();
	void destroyRandomBall();
	void toggleMultithreading();

	static constexpr int N_ENTITIES = 50000;

	lili::ECSRegistry ecs_registry;
	std::vector<lili::Entity> spawned_entities;
	lili::Camera camera;

	std::unique_ptr<lili::Texture> circle_texture;
	std::unique_ptr<lili::SpriteBatch> sprite_batch;
	std::unique_ptr<lili::SystemBase> movement_system;

	bool multithreading_enabled = true;
};
