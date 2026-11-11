#include "app.hpp"

#include <lili2d/components/ui_layout.hpp>
#include <lili2d/core/asset_manager.hpp>
#include <lili2d/entities/rect.hpp>
#include <lili2d/entities/text.hpp>
#include <lili2d/render/common/material_registry.hpp>

using namespace lili;

App::App()
  : Game("hello_shader - Lili2D", 800, 800)
{
    Assets::setHotReloadEnabled(true);
    Renderer* renderer = getRenderer();
    ECSRegistry& ecs_reg = getECSRegistry();
    MaterialRegistry& mat_reg = MaterialRegistry::get();

    Shader* rect_shader = Assets::loadShader(
        "rect_shader",
        "src/rect.vert.hlsl",
        "src/default.frag.hlsl",
        renderer->getDevice()
    );
    rect_pipeline.reset(renderer->createMainGraphicsPipeline(rect_shader));
    Material custom_rect_mat =
        Material(renderer->getTheWhitePixel(), rect_pipeline.get());
    mat_reg.registerMaterial("custom:rect", custom_rect_mat);

    rect = createRect(
        ecs_reg,
        {
            .render2d{
                .size = Vec2(400.0f, 400.0f),
                .material_id = mat_reg.getMaterialID("custom:rect"),
                .render_pass = lili::RenderLayer::UI,
            },
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
    Material custom_text_mat =
        Material(renderer->getTheWhitePixel(), text_pipeline.get());
    mat_reg.registerMaterial("custom:text", custom_text_mat);

    text = createText(
        ecs_reg,
        "Yay, shaders :D",
        {
            .transform{
                .pos = Vec2(250.0f, 75.0f),
                .scale = Vec2(3.0f, 3.0f),
            },
            .render2d = {
                .material_id = mat_reg.getMaterialID("custom:text"),
                .render_pass = lili::RenderLayer::UI,
            },
        }
    );
    ecs_reg.addComponents(
        text,
        UILayoutComponent{
            .offset = Vec2(0.0f, 30.0f),
            .anchor = lili::Anchor::TOP,
            .pivot = lili::Pivot::TOP,
        }
    );

    text_info = createText(
        ecs_reg,
        "SPACE: toggle custom shaders",
        {
            .transform{ .scale = Vec2(3.0f, 3.0f) },
            .render2d{ .render_pass = lili::RenderLayer::UI },
        }
    );
    ecs_reg.addComponents(
        text_info,
        UILayoutComponent{
            .offset = Vec2(0.0f, -30.0f),
            .anchor = lili::Anchor::BOTTOM,
            .pivot = lili::Pivot::BOTTOM,
        }
    );

    use_custom_shaders = true;
}

void
App::toggleCustomShaders()
{
    ECSRegistry& ecs_reg = getECSRegistry();
    StringArena& string_arena = ecs_reg.getStringArena();
    MaterialRegistry& mat_reg = MaterialRegistry::get();

    use_custom_shaders = !use_custom_shaders;

    auto& rect_render2d = ecs_reg.getComponent<Render2DComponent>(rect);
    auto& text_render2d = ecs_reg.getComponent<Render2DComponent>(text);

    std::string_view new_text =
        use_custom_shaders ? "Yay, shaders :D" : "Oh, no shaders :(";

    if (use_custom_shaders) {
        rect_render2d.material_id = mat_reg.getMaterialID("custom:rect");
        text_render2d.material_id = mat_reg.getMaterialID("custom:text");
    } else {
        rect_render2d.material_id = mat_reg.getMaterialID("core:none");
        text_render2d.material_id = mat_reg.getMaterialID("core:none");
    }

    auto& text_comp = ecs_reg.getComponent<TextComponent>(text);
    string_arena.free(text_comp);
    string_arena.append(new_text, text_comp);
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

    Material& rect_mat = MaterialRegistry::get().getMaterial("custom:rect");
    rect_mat.setVertexUniforms(
        RectUB{
            .time = getClock().getTime(),
            .amplitude = 40.0f,
            .frequency = 20.0f,
            .speed = 6.0f,
        }
    );

    Material& text_mat = MaterialRegistry::get().getMaterial("custom:text");
    text_mat.setVertexUniforms(
        TextUB{
            .speed = 2.0f,
            .time = getClock().getTime(),
        }
    );
}
