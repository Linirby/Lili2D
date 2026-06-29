#include "app.hpp"
#include <cmath>

App::App() {
	window = std::make_unique<lili::Window>(
		"hello_sprite_batch - Lili2D", 768, 640
	);
	renderer = std::make_unique<lili::Renderer>(window.get());
	clock = lili::Clock(20.0f);
	camera.setZoom(4.0f);
	renderer->setCamera(&camera);

	lili::Vec2 tile_render_size = lili::Vec2(16, 16);
	tilemap = std::make_unique<lili::TileMap>(lili::Vec2(tile_render_size));

	env_atlas = lili::AtlasMap(renderer.get(), "assets/environment.png");
	env_atlas.slice(3, 2);

	lili::TileRegistry &registry = lili::TileRegistry::get();
	registry.registerTile("grass:light", lili::Tile(env_atlas.getSliceUV(0)));
	registry.registerTile("grass:medium", lili::Tile(env_atlas.getSliceUV(1)));
	registry.registerTile("grass:dark", lili::Tile(env_atlas.getSliceUV(2)));
	registry.registerTile("dirt:light", lili::Tile(env_atlas.getSliceUV(3)));
	lili::Tile invisible_solid;
	invisible_solid.is_solid = true;
	lili::TileRegistry::get().registerTile(
		"solid_invisible",
		std::move(invisible_solid)
	);

	int map_width = 150;
	int map_height = 150;

	for (int y = 0; y < map_height; ++y) {
		for (int x = 0; x < map_width; ++x) {
			float noise = std::sin(x * 0.1f) * std::cos(y * 0.1f) * 3.0f;
			noise += (
				std::sin(x * 0.05f + 10.0f) * 2.0f +
				std::cos(y * 0.05f + 10.0f) * 2.0f
			);

			int elevation = (int)(noise + 3.0f);
			if (elevation < 0) elevation = 0;
			if (elevation > 5) elevation = 5;

			for (int z = 0; z < elevation; ++z) {
				tilemap->setTile("solid_invisible", lili::Point3(x, y, z));
			}

			std::string tile_name;
			if (elevation >= 4) tile_name = "grass:light";
			else if (elevation >= 2) tile_name = "grass:medium";
			else tile_name = "grass:dark";

			tilemap->setTile(tile_name, lili::Point3(x, y, elevation));
		}
	}



	font = lili::BitmapFont(renderer.get(), "assets/lili_font.png", 16, 6);
	text_infos = lili::Text(
		renderer.get(), &font, "WASD: move | IK: zoom/dezoom"
	);
	text_infos.setRender(lili::RenderLayer::UI);
	text_infos.setPosition({10.0f, 10.0f});
	text_infos.setScale(3.0f);

	running = true;
}

void App::run() {
	while (running) {
		clock.update();
		handleEvents();
		update(clock.getDt());
		render();
	}
}

void App::handleEvents() {
	lili::Event event;
	while (event.poll()) {
		if (
			event.type() == lili::EventType::QUIT ||
			event.keyboard().key == SDLK_ESCAPE
		)
			running = false;
	}
}

void App::update(float dt) {
	keyboard.update();
	lili::Vec2 vel(0, 0);

	if (keyboard.held(SDL_SCANCODE_W))
		vel.y = -1;
	if (keyboard.held(SDL_SCANCODE_S))
		vel.y = 1;
	if (keyboard.held(SDL_SCANCODE_A))
		vel.x = -1;
	if (keyboard.held(SDL_SCANCODE_D))
		vel.x = 1;

	lili::Vec2 camera_pos = camera.getPosition();
	camera.setPosition(camera_pos + (vel * 100.0f * dt));

	if (keyboard.held(SDL_SCANCODE_I))
		camera.setZoom(camera.getZoom() + dt);
	if (keyboard.held(SDL_SCANCODE_K))
		camera.setZoom(camera.getZoom() - dt);
}

void App::render() {
	if (!renderer->beginFrame()) return;

	text_infos.draw();
	tilemap->draw(renderer.get());



	renderer->endFrame();
}
