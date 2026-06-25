#include "app.hpp"

App::App() {
	window = std::make_unique<lili::Window>(
		"hello_sprite_batch - Lili2D", 768, 640
	);
	renderer = std::make_unique<lili::Renderer>(window.get());
	clock = lili::Clock(20.0f);
	camera.setZoom(4.0f);
	renderer->setCamera(&camera);

	env_atlas = lili::AtlasMap(renderer.get(), "assets/environment.png");
	env_atlas.slice(4, 2);

	char_atlas = lili::AtlasMap(renderer.get(), "assets/player.png");
	char_atlas.slice(4, 5);

	anim_idle = lili::Animation(char_atlas.getSliceUVs(0, 4));
	anim_run_right = lili::Animation(char_atlas.getSliceUVs(4, 4));
	anim_run_left = lili::Animation(char_atlas.getSliceUVs(8, 4));
	anim_run_top = lili::Animation(char_atlas.getSliceUVs(12, 4));
	anim_run_bottom = lili::Animation(char_atlas.getSliceUVs(16, 4));

	env_batch = std::make_unique<lili::SpriteBatch>(
		renderer.get(), env_atlas.getTexture()
	);
	char_batch = std::make_unique<lili::SpriteBatch>(
		renderer.get(), char_atlas.getTexture()
	);

	const float TILE_SIZE = 16.0f;
	
	lili::SliceUV slice_floor = env_atlas.getSliceUV(0);
	lili::SliceUV slice_dark_floor = env_atlas.getSliceUV(1);
	lili::SliceUV slice_wall = env_atlas.getSliceUV(2);
	lili::SliceUV slice_corner_tl = env_atlas.getSliceUV(4);
	lili::SliceUV slice_corner_tr = env_atlas.getSliceUV(5);
	lili::SliceUV slice_corner_bl = env_atlas.getSliceUV(6);
	lili::SliceUV slice_corner_br = env_atlas.getSliceUV(7);

	int map_width = 100;
	int map_height = 80;
	
	env_batch->begin();
	for (int y = 0; y < map_height; ++y) {
		for (int x = 0; x < map_width; ++x) {
			lili::SliceUV slice = slice_floor;
			if ((x + y) % 2 == 0)
				slice = slice_dark_floor;

			if (x == 0 && y == 0)
				slice = slice_corner_tl;
			else if (x == map_width - 1 && y == 0)
				slice = slice_corner_tr;
			else if (x == 0 && y == map_height - 1)
				slice = slice_corner_bl;
			else if (x == map_width - 1 && y == map_height - 1)
				slice = slice_corner_br;
			else if (
				x == 0 || x == map_width - 1 ||
				y == 0 || y == map_height - 1
			)
				slice = slice_wall;

			env_batch->draw(
				slice,
				lili::Vec2(
					(x + 1.5 - 50) * TILE_SIZE,
					(y + 1.5 - 40) * TILE_SIZE
				)
			);
		}
	}
	env_batch->end();

	env_batch->setLayer(0.5f);
	char_batch->setLayer(1.0f);

	current_anim = &anim_idle;
	player.anim_player = lili::AnimationPlayer(current_anim);

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
	lili::Vec2 velocity(0, 0);
	
	if (keyboard.held(SDL_SCANCODE_W))
		velocity.y -= 1;
	if (keyboard.held(SDL_SCANCODE_S))
		velocity.y += 1;
	if (keyboard.held(SDL_SCANCODE_A))
		velocity.x -= 1;
	if (keyboard.held(SDL_SCANCODE_D))
		velocity.x += 1;

	if (keyboard.held(SDL_SCANCODE_I))
		camera.setZoom(camera.getZoom() + dt);
	if (keyboard.held(SDL_SCANCODE_K))
		camera.setZoom(camera.getZoom() - dt);

	bool is_moving = (velocity.x != 0 || velocity.y != 0);
	lili::Animation* target_anim = current_anim;

	if (is_moving) {
		float speed = 80.0f;
		player.position.x += velocity.x * speed * dt;
		player.position.y += velocity.y * speed * dt;

		if (velocity.y < 0)
			target_anim = &anim_run_top;
		else if (velocity.y > 0)
			target_anim = &anim_run_bottom;
		else if (velocity.x < 0)
			target_anim = &anim_run_left;
		else if (velocity.x > 0)
			target_anim = &anim_run_right;
	} else
		target_anim = &anim_idle;

	if (current_anim != target_anim) {
		current_anim = target_anim;
		player.anim_player.setAnimation(current_anim);
	}

	player.anim_player.update(dt);
}

void App::render() {
	if (!renderer->beginFrame()) return;

	env_batch->render();

	char_batch->begin();
	char_batch->draw(player.anim_player.getCurrentFrame(), player.position);
	char_batch->end();
	char_batch->render();

	text_infos.draw();

	renderer->endFrame();
}
