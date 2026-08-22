#include "app.hpp"

#include "lili2d/render/passes/pass_types.hpp"
#include "lili2d/render/ui/ui_layout.hpp"

App::App() : lili::Game("hello_shader - Lili2D", 800, 800) {
    lili::Assets::setHotReloadEnabled(true);
    lili::Renderer* renderer = getRenderer();
    lili::Shader* rect_shader = lili::Assets::loadShader(
        "rect_shader", "src/rect.vert.hlsl", "src/rect.frag.hlsl",
        renderer->getDevice()
    );
    rect_pipeline = std::make_unique<lili::MainGraphicsPipeline>(
        renderer->getDevice(), getWindow()->getSdlWindow(), rect_shader
    );
    rect = lili::Rect(
        renderer, lili::RectShape(0.0f, 0.0f, 400.0f, 400.0f),
        lili::Vec4(1.0f, 1.0f, 1.0f, 1.0f)
    );
    rect.getMaterial()->pipeline = rect_pipeline.get();
    rect.setRender(lili::RenderLayer::UI);
    rect.setAnchor(lili::Anchor::CENTER);
    rect.setPivot(lili::Pivot::CENTER);
    rect.setOffset({0.0f, 0.0f});

    lili::Shader* text_shader = lili::Assets::loadShader(
        "text_shader", "src/text.vert.hlsl", "src/text.frag.hlsl",
        renderer->getDevice()
    );
    text_pipeline = std::make_unique<lili::MainGraphicsPipeline>(
        renderer->getDevice(), getWindow()->getSdlWindow(), text_shader
    );
    lili::BitmapFont* font =
        lili::Assets::loadFont("lili_font", renderer, "lili_font.png", 16, 6);
    text = lili::Text(renderer, font, "Yay, shaders :D");
    text.setPosition(lili::Vec2(250.0f, 75.0f));
    text.setScale(3.0f);
    text.getMaterial()->pipeline = text_pipeline.get();
    text.setAnchor(lili::Anchor::TOP);
    text.setPivot(lili::Pivot::TOP);
    text.setOffset({0.0f, 30.0f});
    text.setRender(lili::RenderLayer::UI);

    text_info = lili::Text(renderer, font, "SPACE: toggle custom shaders");
    text_info.setAnchor(lili::Anchor::BOTTOM);
    text_info.setPivot(lili::Pivot::BOTTOM);
    text_info.setOffset({0.0f, -30.0f});
    text_info.setScale(3.0f);
    text_info.setRender(lili::RenderLayer::UI);

    toggle_custom_shaders = true;
}

void
App::onEvent(const lili::Event& event) {
    lili::KeyboardEvent kb = event.keyboard();

    if (event.type() == lili::EventType::KEYBOARD)
        if (kb.action == lili::KeyAction::PRESSED)
            if (kb.key == SDLK_ESCAPE) shutdown();

    if (event.type() == lili::EventType::KEYBOARD) {
        lili::KeyboardEvent keyboard = event.keyboard();
        if (keyboard.action == lili::KeyAction::PRESSED) {
            if (keyboard.key == SDLK_SPACE && keyboard.repeat == false)
                toggle_custom_shaders = !toggle_custom_shaders;
        }
    }
}

void
App::onUpdate(float dt) {
    (void)dt;
    clock.update();
}

void
App::onRender(float alpha) {
    (void)alpha;
    if (toggle_custom_shaders) {
        TextUB text_uniform{};
        text_uniform.speed = 2.0f;
        text_uniform.time = clock.getTime();
        text.getMaterial()->pipeline = text_pipeline.get();
        text.getMaterial()->setVertexUniforms(text_uniform);
        text.setText("Yay, shaders :D");
    } else {
        text.getMaterial()->pipeline = nullptr;
        text.setText("Oh, no shaders :(");
    }
    text.draw();
    text_info.draw();

    if (toggle_custom_shaders) {
        RectUB rect_uniform{};
        rect_uniform.time = clock.getTime();
        rect_uniform.amplitude = 0.2f;
        rect_uniform.frequency = 30.0f;
        rect_uniform.speed = 5.0f;
        rect.getMaterial()->pipeline = rect_pipeline.get();
        rect.getMaterial()->setVertexUniforms(rect_uniform);
    } else
        rect.getMaterial()->pipeline = nullptr;
    rect.draw();
}
