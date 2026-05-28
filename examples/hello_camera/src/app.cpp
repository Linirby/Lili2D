#include "app.hpp"

App::App() {
	window = std::make_unique<lili::Window>("hello_sprite - Lili2D", 600, 400);
	window->set_resizable(true);
	renderer = std::make_unique<lili::Renderer>(window.get());
	font = std::make_unique<lili::BitmapFont>(
		renderer.get(), "lili_font.png", 16, 6
	);
	clock = lili::Clock(20.0f);

	camera = lili::Camera();
	cam_pos = { 250, 250 };
	camera.set_position(cam_pos);
	camera_zoom = camera.get_zoom();
	renderer->set_camera(&camera);

	text_cam_pos = lili::Text(renderer.get(), font.get(), "");
	text_cam_pos.set_scale(2);
	text_cam_pos.set_render(lili::RenderLayer::UI);

	red_rect = lili::Rect(
		renderer.get(),
		lili::RectShape(250, 200, 100, 50),
		lili::Vec4(1, 0, 0, 1)
	);
	green_rect = lili::Rect(
		renderer.get(),
		lili::RectShape(100, 100, 75, 75),
		lili::Vec4(0, 1, 0, 1)
	);
	blue_rect = lili::Rect(
		renderer.get(),
		lili::RectShape(400, 250, 50, 200),
		lili::Vec4(0, 0, 1, 1)
	);

	camera_center = lili::Circle(
		renderer.get(),
		lili::CircleShape(
			lili::Vec2(window->get_width() / 2.0f, window->get_height() / 2.0f),
			5, 16
		),
		lili::Vec4(1, 1, 1, 0.5)
	);
	camera_center.set_render(lili::RenderLayer::UI);

	running = true;
}

void App::run() {
	while (running) {
		clock.update();
		keyboard.update();

		handle_events();
		update(clock.get_dt());
		render();
	}
}

void App::handle_events() {
	lili::Event event;

	while (event.poll())
		if (event.type() == lili::EventType::QUIT)
			running = false;
}

void App::update(float dt) {
	if (keyboard.pressed(SDL_SCANCODE_ESCAPE))
		running = false;

	cam_pos = camera.get_position();
	if (keyboard.held(SDL_SCANCODE_I))
		cam_pos.y -= 100.0f * dt;
	if (keyboard.held(SDL_SCANCODE_K))
		cam_pos.y += 100.0f * dt;
	if (keyboard.held(SDL_SCANCODE_J))
		cam_pos.x -= 100.0f * dt;
	if (keyboard.held(SDL_SCANCODE_L))
		cam_pos.x += 100.0f * dt;
	camera.set_position(cam_pos);

	if (keyboard.held(SDL_SCANCODE_Z))
		camera_zoom += 10.0f * dt;
	if (keyboard.held(SDL_SCANCODE_X))
		camera_zoom -= 10.0f * dt;
	camera.set_zoom(camera_zoom);

	text_cam_pos.set_text(
		"Camera Position: (X=" +
		std::to_string(cam_pos.x) +
		", Y=" +
		std::to_string(cam_pos.y) +
		")"
	);
	text_cam_pos.set_position({ 10, 10 });

	camera_center.set_center(
		lili::Vec2(window->get_width() / 2.0f, window->get_height() / 2.0f)
	);
}

void App::render() {
	if (!renderer->begin_frame()) return;

	text_cam_pos.draw();

	red_rect.draw();
	green_rect.draw();
	blue_rect.draw();

	camera_center.draw();

	renderer->end_frame();
}
