#include "app.hpp"
#include <lili2d/components/render2d.hpp>
#include <lili2d/components/ui_layout.hpp>
#include <lili2d/core/asset_manager.hpp>
#include <lili2d/core/game.hpp>
#include <lili2d/ecs/ecs_registry.hpp>
#include <lili2d/entities/rect.hpp>
#include <lili2d/geometry/vec2.hpp>
#include <lili2d/render/common/material.hpp>
#include <lili2d/render/common/material_registry.hpp>
#include <lili2d/render/gpu/pass_types.hpp>

using namespace lili;

App::App()
  : Game("hello_shader - Lili2D", 800, 800)
{
    Assets::setHotReloadEnabled(true);
    Renderer* renderer = getRenderer();
    ECSRegistry& ecs_reg = getECSRegistry();

    Shader* rect_shader = Assets::loadShader(
        "rect_shader",
        "src/rect.vert.hlsl",
        "src/default.frag.hlsl",
        renderer->getDevice()
    );
    rect_pipeline.reset(renderer->createMainGraphicsPipeline(rect_shader));

    Material custom_rect_mat =
        Material(renderer->getTheWhitePixel(), rect_pipeline.get());
    MaterialRegistry& mat_reg = MaterialRegistry::get();
    mat_reg.registerMaterial("custom:rect", custom_rect_mat);

    rect = createRect(
        ecs_reg,
        {
            .color = Vec4(1.0f, 1.0f, 1.0f, 1.0f),
            .pos = Vec2(0.0f, 0.0f),
            .size = Vec2(400.0f, 400.0f),
            .material_id = mat_reg.getMaterialID("custom:rect"),
            .render_pass = lili::RenderLayer::UI,
        }
    );
    ecs_reg.addComponents(rect, UILayoutComponent{});

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

    use_custom_shaders = true;
}

void
App::toggleCustomShaders()
{
    ECSRegistry& ecs_reg = getECSRegistry();
    MaterialRegistry& mat_reg = MaterialRegistry::get();

    use_custom_shaders = !use_custom_shaders;
    auto& render2d = ecs_reg.getComponent<Render2DComponent>(rect);
    if (use_custom_shaders) {
        render2d.material_id = mat_reg.getMaterialID("custom:rect");
    } else {
        render2d.material_id = mat_reg.getMaterialID("core:none");
    }
    text.getMaterial()->pipeline =
        use_custom_shaders ? text_pipeline.get() : nullptr;
    text.setText(use_custom_shaders ? "Yay, shaders :D" : "Oh, no shaders :(");
}

void
App::onEvent(const Event& event)
{
    Game::onEvent(event);

    if (event.type() == EventType::KEYBOARD) {
        KeyboardEvent kb = event.keyboard();
        if (kb.action == KeyAction::PRESSED && !kb.repeat) {
            if (kb.key == Key::ESCAPE)
                shutdown();
            else if (kb.key == Key::SPACE)
                toggleCustomShaders();
        }
    }
}

void
App::onUpdate(float dt)
{
    Game::onUpdate(dt);

    Material& mat = MaterialRegistry::get().getMaterial("custom:rect");
    mat.setVertexUniforms(
        RectUB{ .time = getClock().getTime(),
                .amplitude = 40.0f,
                .frequency = 20.0f,
                .speed = 6.0f }
    );
}

void
App::onRender(float alpha)
{
    Game::onRender(alpha);

    if (use_custom_shaders)
        text.getMaterial()->setVertexUniforms(
            TextUB{ .speed = 2.0f, .time = clock.getTime() }
        );

    text.draw();
    text_info.draw();
}
