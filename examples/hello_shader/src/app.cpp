#include "app.hpp"

App::App() : lili::Game("hello_shader - Lili2D", 800, 800) {
	lili::ShaderInfo vert_rect_info{};
	vert_rect_info.num_uniform_buffers = 2;  // Slot 0 internal, Slot 1 custom
	lili::ShaderInfo frag_rect_info{};
	frag_rect_info.num_samplers = 1;  // Sampler internal
	rect_shader.reset(renderer->createShader(
		"build/rect.vert.spv",
		"build/rect.frag.spv",
		vert_rect_info,
		frag_rect_info
	));
	rect_pipeline.reset(
		renderer->createMainGraphicsPipeline(rect_shader.get())
	);
	rect = lili::Rect(
		renderer.get(),
		lili::RectShape(200.0f, 200.0f, 400.0f, 400.0f),
		lili::Vec4(1.0f, 1.0f, 1.0f, 1.0f)
	);
	rect.getMaterial()->custom_pipeline = rect_pipeline->getSdlPipeline();

	lili::ShaderInfo vert_text_info{};
	vert_text_info.num_uniform_buffers = 2;
	lili::ShaderInfo frag_text_info{};
	frag_text_info.num_samplers = 1;
	text_shader.reset(renderer->createShader(
		"build/text.vert.spv",
		"build/text.frag.spv",
		vert_text_info,
		frag_text_info
	));
	text_pipeline.reset(
		renderer->createMainGraphicsPipeline(text_shader.get())
	);
	font = std::make_unique<lili::BitmapFont>(
		renderer.get(), "lili_font.png", 16, 6
	);
	text = lili::Text(renderer.get(), font.get(), "Yay, shaders :D");
	text.setPosition(lili::Vec2(250.0f, 75.0f));
	text.setScale(3.0f);
	text.getMaterial()->custom_pipeline = text_pipeline->getSdlPipeline();
	text_info = lili::Text(
		renderer.get(), font.get(), "SPACE: toggle custom shaders"
	);
	text_info.setPosition(lili::Vec2(10.0, window->getHeight() - 32.0f));
	text_info.setScale(3.0f);

	toggle_custom_shaders = true;
}

void App::onEvent(const lili::Event &event) {
	if (event.type() == lili::EventType::KEYBOARD) {
		lili::KeyboardEvent keyboard = event.keyboard();
		if (keyboard.action == lili::KeyAction::PRESSED) {
			if (keyboard.key == SDLK_SPACE && keyboard.repeat == false)
				toggle_custom_shaders = !toggle_custom_shaders;
		}
	}
}

void App::onUpdate(float dt) {
	(void)dt;
	clock.update();
}

void App::onRender(float alpha) {
	(void)alpha;
	if (toggle_custom_shaders) {
		TextUB text_uniform{};
		text_uniform.speed = 2.0f;
		text_uniform.time = clock.getTime();
		text.getMaterial()->custom_pipeline = (
			text_pipeline->getSdlPipeline()
		);
		text.getMaterial()->setVertexUniforms(text_uniform);
		text.setText("Yay, shaders :D");
	} else {
		text.getMaterial()->custom_pipeline = nullptr;
		text.setText("Oh, no shaders :(");
	}
	text.draw();
	text_info.draw();

	rect.getMaterial()->custom_pipeline = (
		toggle_custom_shaders ? rect_pipeline->getSdlPipeline() : nullptr
	);
	if (toggle_custom_shaders) {
		RectUB rect_uniform{};
		rect_uniform.time = clock.getTime();
		rect_uniform.amplitude = 0.2f;
		rect_uniform.frequency = 30.0f;
		rect_uniform.speed = 5.0f;
		rect.getMaterial()->custom_pipeline = (
			rect_pipeline->getSdlPipeline()
		);
		rect.getMaterial()->setVertexUniforms(rect_uniform);
		text.setText("Yay, shaders :D");
	} else {
		text.getMaterial()->custom_pipeline = nullptr;
		text.setText("Oh, no shaders :(");
	}
	rect.draw();
}
