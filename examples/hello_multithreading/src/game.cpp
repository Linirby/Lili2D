#include "game.hpp"
#include "components.hpp"
#include "systems.hpp"
#include "lili2d/core/event.hpp"
#include <SDL3/SDL_keycode.h>
#include <random>
#include <iostream>

App::App() : lili::Game("hello_multithreading - Lili2D", 800, 600) {
	setTps(60.0f);
}

void App::onInit() {
	lili::Window *window = getWindow();
	lili::Renderer *renderer = getRenderer();

	camera = lili::Camera();
	camera.setPosition({window->getWidth() / 2.0f, window->getHeight() / 2.0f});
	renderer->setCamera(&camera);

	circle_texture = std::make_unique<lili::Texture>(
		renderer->getDevice(), "circle.png"
	);
	sprite_batch = std::make_unique<lili::SpriteBatch>(
		renderer, circle_texture.get()
	);

	movement_system = std::make_unique<systems::MovementSystem>(
		static_cast<float>(window->getWidth()),
		static_cast<float>(window->getHeight())
	);

	for (int i = 0; i < N_ENTITIES; ++i)
		spawnRandomBall();

	std::cout <<
		"=== Lili2D Multithreading Demo Instructions ===\n" <<
		"  [SPACE]     : Spawn a new random ball entity\n" <<
		"  [BACKSPACE] : Destroy a random ball entity\n" <<
		"  [M]         : Toggle C++20 Multithreading (Auto Threshold vs "
		"Single-Thread)\n" <<
		"================================================\n";
}

void App::onEvent(const lili::Event &event) {
	if (event.type() == lili::EventType::KEYBOARD) {
		lili::KeyboardEvent kb = event.keyboard();
		if (kb.action == lili::KeyAction::PRESSED) {
			if (kb.key == SDLK_ESCAPE)
				shutdown();
			else if (kb.key == SDLK_SPACE)
				spawnRandomBall();
			else if (kb.key == SDLK_BACKSPACE)
				destroyRandomBall();
			else if (kb.key == SDLK_M)
				toggleMultithreading();
			std::cout << "Total entities: " << spawned_entities.size() << "\n";
		}
	}
}

void App::onUpdate(float dt) {
	movement_system->run(ecs_registry, dt, getThreadPool());
}

void App::onRender(float alpha) {
	(void)alpha;

	sprite_batch->begin();

	auto &pos_pool = ecs_registry.getPool<PositionComponent>();
	const auto &entities = pos_pool.getEntities();
	const auto &positions = pos_pool.getComponents();

	for (size_t i = 0; i < entities.size(); ++i) {
		lili::Entity entity = entities[i];
		if (ecs_registry.hasComponent<RenderComponent>(entity)) {
			const auto &pos = positions[i];
			auto &render = ecs_registry.getComponent<RenderComponent>(entity);

			sprite_batch->draw(
				render.slice,
				pos.value,
				{ render.radius * 2.0f, render.radius * 2.0f },
				0.0f,
				render.color
			);
		}
	}

	sprite_batch->end();
	sprite_batch->draw();
}

void App::spawnRandomBall() {
	static std::random_device rd;
	static std::mt19937 gen(rd());
	lili::Window *window = getWindow();
	std::uniform_real_distribution<float> disX(
		50.0f, static_cast<float>(window->getWidth()) - 50.0f
	);
	std::uniform_real_distribution<float> disY(
		50.0f, static_cast<float>(window->getHeight()) - 50.0f
	);
	std::uniform_real_distribution<float> disVel(-200.0f, 200.0f);
	std::uniform_real_distribution<float> disRadius(5.0f, 15.0f);
	std::uniform_real_distribution<float> disColor(0.2f, 1.0f);

	lili::Vec2 pos(disX(gen), disY(gen));
	lili::Vec2 vel(disVel(gen), disVel(gen));
	float radius = disRadius(gen);
	lili::Vec4 color(disColor(gen), disColor(gen), disColor(gen), 1.0f);

	lili::Entity ent = ecs_registry.createEntity();
	ecs_registry.emplaceComponent<PositionComponent>(ent, pos);
	ecs_registry.emplaceComponent<VelocityComponent>(ent, vel);
	ecs_registry.emplaceComponent<RenderComponent>(ent,
		lili::SliceUV(
			circle_texture.get(),
			0.0f, 0.0f,
			1.0f, 1.0f,
			1.0f, 1.0f
		),
		color,
		radius
	);
	spawned_entities.push_back(ent);
}

void App::destroyRandomBall() {
	if (spawned_entities.empty()) {
		return;
	}

	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_int_distribution<size_t> dis(0, spawned_entities.size() - 1);
	size_t index = dis(gen);
	lili::Entity ent = spawned_entities[index];

	ecs_registry.destroyEntity(ent);
	spawned_entities.erase(spawned_entities.begin() + index);
}

void App::toggleMultithreading() {
	lili::EngineConfig current = getConfig();
	if (current.profile == lili::PerformanceProfile::YES) {
		current.profile = lili::PerformanceProfile::CORRECT;
		configure(current);
		std::cout <<
			">> Switch to Profile: CORRECT"
			" (Multithreaded ECS + Mailbox present)\n";
	} else {
		current.profile = lili::PerformanceProfile::YES;
		current.potato_present_mode = SDL_GPU_PRESENTMODE_VSYNC;
		configure(current);
		std::cout <<
			">> Switch to Profile: YES"
			" (Single-threaded ECS + VSync present)\n";
	}
}
