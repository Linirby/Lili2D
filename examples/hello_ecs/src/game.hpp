#pragma once

#include <lili2d/lili2d.hpp>

#ifndef N_ENTITIES
# define N_ENTITIES 1000
#endif  // N_ENTITIES

class App : public lili::Game {
public:
	App();

private:
	lili::Camera camera;
	lili::ECSRegistry ecs_registry;

	std::unique_ptr<lili::Texture> circle_texture;
	std::unique_ptr<lili::SpriteBatch> sprite_batch;
	std::vector<lili::Entity> spawned_entities;

	void onEvent(const lili::Event &event) override;
	void onUpdate(float dt) override;
	void onRender(float alpha) override;

	void spawnRandomBall();
	void destroyRandomBall();
	void toggleRandomBallVelocity();
};
