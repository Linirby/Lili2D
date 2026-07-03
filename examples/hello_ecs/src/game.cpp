#include "game.hpp"
#include "components.hpp"
#include "entities.hpp"
#include "systems.hpp"
#include <random>
#include <iostream>

Game::Game() {
	window = std::make_unique<lili::Window>("hello_ecs - Lili2D", 600, 400);
	renderer = std::make_unique<lili::Renderer>(window.get());

	clock = lili::Clock(20.0f);
	camera = lili::Camera();
	camera.setPosition({window->getWidth() / 2.0f, window->getHeight() / 2.0f});
	renderer->setCamera(&camera);

	running = true;

	for (int i = 0; i < 10; ++i)
		spawnRandomBall();

	std::cout <<
		"=== Lili2D ECS Demo Instructions ===\n" <<
		"  [SPACE]     : Spawn a new random ball entity\n" <<
		"  [BACKSPACE] : Destroy a random ball entity\n" <<
		"  [T]         : Toggle (remove/add) velocity component of a random ball\n" <<
		"====================================\n";
}

void Game::run() {
	while (running) {
		clock.update();

		handleEvents();
		update(clock.getDt());
		render();
	}
}

void Game::handleEvents() {
	lili::Event event;

	while (event.poll()) {
		if (event.type() == lili::EventType::QUIT) {
			running = false;
		}

		if (event.type() == lili::EventType::KEYBOARD) {
			lili::KeyboardEvent kb = event.keyboard();
			if (kb.action == lili::KeyAction::PRESSED) {
				if (kb.key == SDLK_ESCAPE) {
					running = false;
				} else if (kb.key == SDLK_SPACE) {
					spawnRandomBall();
				} else if (kb.key == SDLK_BACKSPACE) {
					destroyRandomBall();
				} else if (kb.key == SDLK_T) {
					toggleRandomBallVelocity();
				}
			}
		}
	}
}

void Game::update(float dt) {
	systems::updateMovement(
		ecs_registry,
		dt,
		static_cast<float>(window->getWidth()),
		static_cast<float>(window->getHeight())
	);
}

void Game::render() {
	if (!renderer->beginFrame()) return;
	systems::renderEntities(ecs_registry);
	renderer->endFrame();
}

void Game::spawnRandomBall() {
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis_x(
		50.0f, static_cast<float>(window->getWidth()) - 50.0f
	);
	std::uniform_real_distribution<float> dis_y(
		50.0f, static_cast<float>(window->getHeight()) - 50.0f
	);
	std::uniform_real_distribution<float> dis_vel(-200.0f, 200.0f);
	std::uniform_real_distribution<float> dis_radius(10.0f, 30.0f);
	std::uniform_real_distribution<float> dis_color(0.2f, 1.0f);

	lili::Vec2 pos(dis_x(gen), dis_y(gen));
	lili::Vec2 vel(dis_vel(gen), dis_vel(gen));
	float radius = dis_radius(gen);
	lili::Vec4 color(dis_color(gen), dis_color(gen), dis_color(gen), 1.0f);

	lili::Entity ent = entities::spawnBall(
		ecs_registry, renderer.get(), pos, vel, radius, color
	);
	spawned_entities.push_back(ent);
	std::cout <<
		"Spawned entity ID: " << lili::getEntityID(ent) <<
		" (Gen: " << static_cast<int>(lili::getEntityGen(ent)) <<
		") | Total: " << spawned_entities.size() << "\n";
}

void Game::destroyRandomBall() {
	if (spawned_entities.empty()) {
		std::cout << "No entities to destroy!\n";
		return;
	}

	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_int_distribution<size_t> dis(0, spawned_entities.size() - 1);
	size_t index = dis(gen);
	lili::Entity ent = spawned_entities[index];

	std::cout <<
		"Destroying entity ID: " << lili::getEntityID(ent) <<
		" (Gen: " << static_cast<int>(lili::getEntityGen(ent)) << ")\n";

	ecs_registry.destroyEntity(ent);
	spawned_entities.erase(spawned_entities.begin() + index);
	std::cout << "Total active entities: " << spawned_entities.size() << "\n";
}

void Game::toggleRandomBallVelocity() {
	if (spawned_entities.empty()) {
		std::cout << "No entities to toggle!\n";
		return;
	}

	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_int_distribution<size_t> dis(0, spawned_entities.size() - 1);
	size_t index = dis(gen);
	lili::Entity ent = spawned_entities[index];

	if (ecs_registry.hasComponent<VelocityComponent>(ent)) {
		std::cout <<
			"Removing Velocity from entity ID: " <<
			lili::getEntityID(ent) <<
			" (it will stop moving)\n";
		ecs_registry.removeComponent<VelocityComponent>(ent);
	} else {
		std::uniform_real_distribution<float> dis_vel(-200.0f, 200.0f);
		lili::Vec2 new_vel(dis_vel(gen), dis_vel(gen));
		std::cout <<
			"Adding Velocity component back to entity ID: " <<
			lili::getEntityID(ent) <<
			" (it will start moving)\n";
		ecs_registry.emplaceComponent<VelocityComponent>(ent, new_vel);
	}
}
