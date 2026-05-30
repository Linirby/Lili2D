#include "app.hpp"

App::App() {
	window = std::make_unique<lili::Window>("hello_sprite - Lili2D", 1024, 576);
	window->set_resizable(true);

	renderer = std::make_unique<lili::Renderer>(window.get());

	font = std::make_unique<lili::BitmapFont>(
		renderer.get(), "lili_font.png", 16, 6
	);

	layer_1 = lili::Sprite(renderer.get(), "layer_1.png");
	layer_1.set_layer(1);
	layer_2 = lili::Sprite(renderer.get(), "layer_2.png");
	layer_2.set_layer(0);
	layer_3 = lili::Sprite(renderer.get(), "layer_3.png");
	layer_3.set_layer(-1);

	red_square = lili::Rect(
		renderer.get(),
		lili::RectShape(100, 350, 64, 96),
		lili::Vec4(1, 0, 0, 1)
	);
	red_square_layer = 1;
	red_square.set_layer(red_square_layer);

	text_current_layer = lili::Text(renderer.get(), font.get(), "");
	text_current_layer.set_scale(2);
	text_current_layer.set_render(lili::RenderLayer::UI);
	text_control_info = lili::Text(
		renderer.get(), font.get(), "I/K: Increase/decrease red rect layer"
	);
	text_control_info.set_scale(2);
	text_control_info.set_render(lili::RenderLayer::UI);

	running = true;
}

void App::run() {
	while (running) {
		keyboard.update();

		handle_events();
		render();
	}
}

void App::handle_events() {
	lili::Event event;

	while (event.poll())
		if (event.type() == lili::EventType::QUIT)
			running = false;

	if (keyboard.pressed(SDL_SCANCODE_K))
		red_square_layer -= 1;
	if (keyboard.pressed(SDL_SCANCODE_I))
		red_square_layer += 1;
	text_current_layer.set_text("Layer: " + std::to_string(red_square_layer));
	red_square.set_layer(red_square_layer);
}

void App::render() {
	if (!renderer->begin_frame()) return;

	red_square.draw();

	layer_1.set_size(window->get_size());
	layer_1.draw();
	layer_2.set_size(window->get_size());
	layer_2.draw();
	layer_3.set_size(window->get_size());
	layer_3.draw();

	text_current_layer.set_position(lili::Vec2(10, window->get_height() - 44));
	text_current_layer.draw();
	text_control_info.set_position(lili::Vec2(10, window->get_height() - 22));
	text_control_info.draw();

	renderer->end_frame();
}
