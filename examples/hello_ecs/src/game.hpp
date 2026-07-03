#pragma once

#include <lili2d/lili2d.hpp>

class Game {
public:
	Game();
	void run();

private:
	std::unique_ptr<lili::Window> window;
	std::unique_ptr<lili::Renderer> renderer;

	lili::Clock clock;
	lili::Camera camera;

	lili::ECSRegistry ecs_registry;
	std::vector<lili::Entity> spawned_entities;

	bool running;

	void handleEvents();
	void update(float dt);
	void render();

	void spawnRandomBall();
	void destroyRandomBall();
	void toggleRandomBallVelocity();
};
