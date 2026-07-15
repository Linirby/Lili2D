#include "app.hpp"
#include "lili2d/core/thread_pool.hpp"
#include <cmath>
#include <memory>

App::App() : lili::Game("hello_sprite_batch - Lili2D", 768, 640) {
	setTps(20.0f);
	lili::Window *window = getWindow();
	lili::Renderer *renderer = getRenderer();

	camera.setZoom(4.0f);
	camera.setPosition({(float)window->getWidth(), (float)window->getHeight()});
	renderer->setCamera(&camera);

	thread_pool = std::make_unique<lili::ThreadPool>();

	lili::Vec2 tile_render_size = lili::Vec2(16, 16);
	tilemap = std::make_unique<lili::TileMap>(lili::Vec2(tile_render_size));

	env_atlas = lili::AtlasMap(renderer, "assets/environment.png");
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

	int map_width = 1500;
	int map_height = 1500;

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

			for (int z = 0; z < elevation; ++z)
				tilemap->setTile("solid_invisible", lili::Point3(x, y, z));

			std::string tile_name;
			if (elevation >= 4) tile_name = "grass:light";
			else if (elevation >= 2) tile_name = "grass:medium";
			else tile_name = "grass:dark";

			tilemap->setTile(tile_name, lili::Point3(x, y, elevation));
		}
	}

	font = lili::BitmapFont(renderer, "assets/lili_font.png", 16, 6);
	text_infos = lili::Text(
		renderer, &font, "WASD: move | IK: zoom/dezoom"
	);
	text_infos.setRender(lili::RenderLayer::UI);
	text_infos.setPosition({10.0f, 10.0f});
	text_infos.setScale(3.0f);
}

void App::onEvent(const lili::Event &event) {
	lili::KeyboardEvent kb = event.keyboard();

	if (event.type() == lili::EventType::KEYBOARD)
		if (kb.action == lili::KeyAction::PRESSED)
			if (kb.key == SDLK_ESCAPE)
				shutdown();
}

void App::onUpdate(float dt) {
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
		camera.setZoom(std::max(0.01f, camera.getZoom() - dt));
}



void App::onRender(float alpha) {
	(void)alpha;
	text_infos.draw();
	tilemap->draw(getRenderer(), thread_pool.get());
}
