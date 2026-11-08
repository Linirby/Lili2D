#include "main_menu.hpp"

#include "another_menu.hpp"
#include "yet_again_menu.hpp"
#include <lili2d/components/ui_layout.hpp>
#include <lili2d/entities/text.hpp>

using namespace lili;

MainScene::MainScene()
  : Scene("MainMenu")
{
    ECSRegistry& ecs_reg = *getECSRegistry();

    text = createText(
        ecs_reg,
        "MainMenu Scene",
        {
            .transform{ .scale = { 3.0f, 3.0f } },
            .render2d{ .render_pass = lili::RenderLayer::UI },
        }
    );
    ecs_reg.addComponents(
        text,
        UILayoutComponent{
            .anchor = lili::Anchor::CENTER,
            .pivot = lili::Pivot::BOTTOM,
        }
    );

    info_text = createText(
        ecs_reg,
        "J and L to change",
        {
            .transform{ .scale = { 2.0f, 2.0f } },
            .render2d{ .render_pass = lili::RenderLayer::UI },
        }
    );
    ecs_reg.addComponents(
        info_text,
        UILayoutComponent{
            .anchor = lili::Anchor::CENTER,
            .pivot = lili::Pivot::TOP,
        }
    );
}

void
MainScene::handleEvents(const Event& event)
{
    if (event.type() == EventType::KEYBOARD) {
        KeyboardEvent kb = event.keyboard();
        if (kb.action == KeyAction::PRESSED && !kb.repeat) {
            if (kb.key == Key::ESCAPE)
                manager->pop();
            else if (kb.key == Key::L)
                manager->changeScene(std::make_unique<AnotherScene>());
            else if (kb.key == Key::J)
                manager->changeScene(std::make_unique<YetAgainScene>());
        }
    }
}
