#include "app.hpp"

App::App() {
	window = std::make_unique<lili::Window>("hello_shader - Lili2D", 800, 800);
	renderer = std::make_unique<lili::Renderer>(window.get());
	clock = lili::Clock();

	lili::ShaderInfo vert_rect_info{};
	vert_rect_info.num_uniform_buffers = 2;  // Slot 0 internal, Slot 1 custom
	lili::ShaderInfo frag_rect_info{};
	frag_rect_info.num_samplers = 1;  // Sampler internal
	rect_shader.reset(renderer->create_shader(
		"build/rect.vert.spv", 
		"build/rect.frag.spv",
		vert_rect_info,
		frag_rect_info
	));
	rect_pipeline.reset(renderer->create_world_pipeline(rect_shader.get()));
	rect = lili::Rect(
		renderer.get(),
		lili::RectShape(200.0f, 200.0f, 400.0f, 400.0f),
		lili::Vec4(1.0f, 1.0f, 1.0f, 1.0f)
	);
	rect.get_material()->custom_pipeline = rect_pipeline->get_sdl_pipeline();

	lili::ShaderInfo vert_text_info{};
	vert_text_info.num_uniform_buffers = 2;
	lili::ShaderInfo frag_text_info{};
	frag_text_info.num_samplers = 1;
	text_shader.reset(renderer->create_shader(
		"build/text.vert.spv",
		"build/text.frag.spv",
        vert_text_info,
		frag_text_info
	));
	text_pipeline.reset(renderer->create_world_pipeline(text_shader.get()));
	font = std::make_unique<lili::BitmapFont>(
		renderer.get(), "lili_font.png", 16, 6
	);
	text = lili::Text(renderer.get(), font.get(), "Yay, shaders :D");
	text.set_position(lili::Vec2(250.0f, 75.0f));
	text.set_scale(3.0f);
	text.get_material()->custom_pipeline = text_pipeline->get_sdl_pipeline();
	text_info = lili::Text(
		renderer.get(), font.get(), "SPACE: toggle custom shaders"
	);
	text_info.set_position(lili::Vec2(10.0, window->get_height() - 32.0f));
	text_info.set_scale(3.0f);

	toggle_custom_shaders = true;

	running = true;
}

void App::run() {
	while (running) {
		handle_events();
		render();
	}
}

void App::handle_events() {
	lili::Event event;

	while (event.poll()) {
		lili::KeyboardEvent keyboard = event.keyboard();

		if (event.type() == lili::EventType::QUIT)
			running = false;
		if (keyboard.action == lili::KeyAction::PRESSED) {
			if (keyboard.key == SDLK_ESCAPE)
				running = false;
			if (keyboard.key == SDLK_SPACE && keyboard.repeat == false)
				toggle_custom_shaders = !toggle_custom_shaders;
		}
	}
}

void App::render() {
	if (!renderer->begin_frame()) return;

	if (toggle_custom_shaders) {
		TextUB text_uniform{};
		text_uniform.speed = 2.0f;
		text_uniform.time = clock.get_time();
		text.get_material()->custom_pipeline = (
			text_pipeline->get_sdl_pipeline()
		);
		text.get_material()->set_vertex_uniforms(text_uniform);
		text.set_text("Yay, shaders :D");
	} else {
		text.get_material()->custom_pipeline = nullptr;
		text.set_text("Oh, no shaders :(");
	}
	text.draw();
	text_info.draw();

	rect.get_material()->custom_pipeline = toggle_custom_shaders ? rect_pipeline->get_sdl_pipeline() : nullptr;
	if (toggle_custom_shaders) {
		RectUB rect_uniform{};
		rect_uniform.time = clock.get_time();
		rect_uniform.amplitude = 0.2f;
		rect_uniform.frequency = 30.0f;
		rect_uniform.speed = 5.0f;
		rect.get_material()->custom_pipeline = (
			rect_pipeline->get_sdl_pipeline()
		);
		rect.get_material()->set_vertex_uniforms(rect_uniform);
		text.set_text("Yay, shaders :D");
	} else {
		text.get_material()->custom_pipeline = nullptr;
		text.set_text("Oh, no shaders :(");
	}
	rect.draw();

	renderer->end_frame();
}
