#include "app.hpp"

using namespace lili;

App::App()
  : Game("hello_shader - Lili2D", 800, 800)
{
    Assets::setHotReloadEnabled(true);
    Renderer* renderer = getRenderer();

    Shader* rect_shader = Assets::loadShader(
        "rect_shader",
        "src/rect.vert.hlsl",
        "src/default.frag.hlsl",
        renderer->getDevice()
    );
    rect_pipeline.reset(renderer->createMainGraphicsPipeline(rect_shader));
    rect = Rect(
        renderer,
        RectShape(0.0f, 0.0f, 400.0f, 400.0f),
        Vec4(1.0f, 1.0f, 1.0f, 1.0f)
    );
    rect.getMaterial()->pipeline = rect_pipeline.get();
    rect.setRender(RenderLayer::UI);
    rect.setAnchor(Anchor::CENTER);
    rect.setPivot(Pivot::CENTER);
    rect.setOffset({ 0.0f, 0.0f });

    Shader* text_shader = Assets::loadShader(
        "text_shader",
        "src/text.vert.hlsl",
        "src/default.frag.hlsl",
        renderer->getDevice()
    );
    text_pipeline.reset(renderer->createMainGraphicsPipeline(text_shader));
    BitmapFont* default_font = renderer->getDefaultFont();
    text = Text(renderer, default_font, "Yay, shaders :D");
    text.setPosition(Vec2(250.0f, 75.0f));
    text.setScale(3.0f);
    text.getMaterial()->pipeline = text_pipeline.get();
    text.setAnchor(Anchor::TOP);
    text.setPivot(Pivot::TOP);
    text.setOffset({ 0.0f, 30.0f });
    text.setRender(RenderLayer::UI);

    text_info = Text(renderer, default_font, "SPACE: toggle custom shaders");
    text_info.setAnchor(Anchor::BOTTOM);
    text_info.setPivot(Pivot::BOTTOM);
    text_info.setOffset({ 0.0f, -30.0f });
    text_info.setScale(3.0f);
    text_info.setRender(RenderLayer::UI);

    toggle_custom_shaders = true;
}

void
App::onEvent(const Event& event)
{
    Game::onEvent(event);
    if (event.type() == EventType::KEYBOARD) {
        KeyboardEvent kb = event.keyboard();
        if (kb.action == KeyAction::PRESSED && !kb.repeat) {
            if (kb.key == Key::ESCAPE) {
                shutdown();
            } else if (kb.key == Key::SPACE) {
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
App::onUpdate(float dt)
{
    (void)dt;
    clock.update();
}

void
App::onRender(float alpha)
{
    (void)alpha;
    if (toggle_custom_shaders) {
        text.getMaterial()->setVertexUniforms(
            TextUB{ .speed = 2.0f, .time = clock.getTime() }
        );
        rect.getMaterial()->setVertexUniforms(
            RectUB{ .time = clock.getTime(),
                    .amplitude = 0.2f,
                    .frequency = 30.0f,
                    .speed = 5.0f }
        );
    }

    text.draw();
    text_info.draw();
    rect.draw();
}
