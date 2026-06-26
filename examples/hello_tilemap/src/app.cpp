#include "app.hpp"

App::App() {
	window = std::make_unique<lili::Window>(
		"hello_sprite_batch - Lili2D", 768, 640
	);
	renderer = std::make_unique<lili::Renderer>(window.get());
	clock = lili::Clock(20.0f);
	camera.setZoom(4.0f);
	renderer->setCamera(&camera);

    tilemap = std::make_unique<lili::TileMap>(lili::Vec2(16, 16));

	env_atlas = lili::AtlasMap(renderer.get(), "assets/environment.png");
	env_atlas.slice(4, 2);

	lili::TileRegistry &registry = lili::TileRegistry::get();
	registry.registerTile("floor:base", lili::Tile(env_atlas.getSliceUV(0)));
	registry.registerTile("floor:dark", lili::Tile(env_atlas.getSliceUV(1)));
	registry.registerTile("wall:base", lili::Tile(env_atlas.getSliceUV(2)));
	registry.registerTile("wall:tl", lili::Tile(env_atlas.getSliceUV(4)));
	registry.registerTile("wall:tr", lili::Tile(env_atlas.getSliceUV(5)));
	registry.registerTile("wall:bl", lili::Tile(env_atlas.getSliceUV(6)));
	registry.registerTile("wall:br", lili::Tile(env_atlas.getSliceUV(7)));

	int map_width = 100;
	int map_height = 80;
	for (int y = 0; y < map_height; ++y) {
		for (int x = 0; x < map_width; ++x) {
			std::string tile_name = "floor:base";
			if ((x + y) % 2 == 0)
				tile_name = "floor:dark";
			if (x == 0 && y == 0)
				tile_name = "wall:tl";
			else if (x == map_width - 1 && y == 0)
				tile_name = "wall:tr";
			else if (x == 0 && y == map_height - 1)
				tile_name = "wall:bl";
			else if (x == map_width - 1 && y == map_height - 1)
				tile_name = "wall:br";
			else if (
				x == 0 || x == map_width - 1 ||
				y == 0 || y == map_height - 1
			)
				tile_name = "wall:base";
			tilemap->setTile(tile_name, lili::Point3(x, y, 0));
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
