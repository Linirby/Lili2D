#include "app.hpp"

App::App() {
	window = std::make_unique<lili::Window>("hello_layer - Lili2D", 1024, 576);
	renderer = std::make_unique<lili::Renderer>(window.get());

	font = std::make_unique<lili::BitmapFont>(
		renderer.get(), "lili_font.png", 16, 6
	);

	layer_1 = lili::Sprite(renderer.get(), "layer_1.png");
	layer_1.setLayer(1);
	layer_2 = lili::Sprite(renderer.get(), "layer_2.png");
	layer_2.setLayer(0);
	layer_3 = lili::Sprite(renderer.get(), "layer_3.png");
	layer_3.setLayer(-1);

	red_square = lili::Rect(
		renderer.get(),
		lili::RectShape(100, 350, 64, 96),
		lili::Vec4(1, 0, 0, 1)
	);
	red_square_layer = 1;
	red_square.setLayer(red_square_layer);

	text_current_layer = lili::Text(renderer.get(), font.get(), "");
	text_current_layer.setScale(2);
	text_current_layer.setRender(lili::RenderLayer::UI);
	text_control_info = lili::Text(
		renderer.get(), font.get(), "I/K: Increase/decrease red rect layer"
	);
	text_control_info.setScale(2);
	text_control_info.setRender(lili::RenderLayer::UI);

	running = true;
}

void App::run() {
	while (running) {
		keyboard.update();

		handleEvents();
		render();
	}
}

void App::handleEvents() {
	lili::Event event;

	while (event.poll())
		if (event.type() == lili::EventType::QUIT)
			running = false;

	if (keyboard.pressed(SDL_SCANCODE_ESCAPE))
		running = false;
	if (keyboard.pressed(SDL_SCANCODE_K))
		red_square_layer -= 1;
	if (keyboard.pressed(SDL_SCANCODE_I))
		red_square_layer += 1;
	text_current_layer.setText("Layer: " + std::to_string(red_square_layer));
	red_square.setLayer(red_square_layer);
}

void App::render() {
	if (!renderer->beginFrame()) return;

	red_square.draw();

	layer_1.setSize(window->getSize());
	layer_1.draw();
	layer_2.setSize(window->getSize());
	layer_2.draw();
	layer_3.setSize(window->getSize());
	layer_3.draw();

	text_current_layer.setPosition(lili::Vec2(10, window->getHeight() - 44));
	text_current_layer.draw();
	text_control_info.setPosition(lili::Vec2(10, window->getHeight() - 22));
	text_control_info.draw();

	renderer->endFrame();
}
