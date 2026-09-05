#include "app.hpp"

App::App() : lili::Game("hello_shader - Lili2D", 800, 800) {
    lili::Assets::setHotReloadEnabled(true);
    lili::Renderer* renderer = getRenderer();

    lili::Shader* rect_shader = lili::Assets::loadShader(
        "rect_shader", "src/rect.vert.hlsl", "src/default.frag.hlsl",
        renderer->getDevice()
    );
    rect_pipeline.reset(renderer->createMainGraphicsPipeline(rect_shader));
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
        "text_shader", "src/text.vert.hlsl", "src/default.frag.hlsl",
        renderer->getDevice()
    );
    text_pipeline.reset(renderer->createMainGraphicsPipeline(text_shader));
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
    lili::Game::onEvent(event);
    if (event.type() == lili::EventType::KEYBOARD) {
        lili::KeyboardEvent kb = event.keyboard();
        if (kb.action == lili::KeyAction::PRESSED && !kb.repeat) {
            if (kb.key == lili::Key::ESCAPE) {
                shutdown();
            } else if (kb.key == lili::Key::SPACE) {
                toggle_custom_shaders = !toggle_custom_shaders;
                rect.getMaterial()->pipeline =
                    toggle_custom_shaders ? rect_pipeline.get() : nullptr;
                text.getMaterial()->pipeline =
                    toggle_custom_shaders ? text_pipeline.get() : nullptr;
                text.setText(
                    toggle_custom_shaders ? "Yay, shaders :D"
                                          : "Oh, no shaders :("
                );
            }
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
        text.getMaterial()->setVertexUniforms(
            TextUB{.speed = 2.0f, .time = clock.getTime()}
        );
        rect.getMaterial()->setVertexUniforms(
            RectUB{
                .time = clock.getTime(),
                .amplitude = 0.2f,
                .frequency = 30.0f,
                .speed = 5.0f
            }
        );
    }

    text.draw();
    text_info.draw();
    rect.draw();
}
