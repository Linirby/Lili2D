#include "another_menu.hpp"

#include "main_menu.hpp"
#include "yet_again_menu.hpp"

using namespace lili;

AnotherScene::AnotherScene(Renderer* renderer)
  : Scene("AnotherMenu", renderer)
{
    BitmapFont* font =
        Assets::loadFont("lili_font", renderer, "lili_font.png", 16, 6);

    text = Text(renderer, font, "AnotherMenu Scene");
    text.setScale(3.0f);
    text.setRender(RenderLayer::UI);
    text.setAnchor(Anchor::CENTER);
    text.setPivot(Pivot::BOTTOM);

    info_text = Text(renderer, font, "J and L to change");
    info_text.setScale(2.0f);
    info_text.setRender(RenderLayer::UI);
    info_text.setAnchor(Anchor::CENTER);
    info_text.setPivot(Pivot::TOP);
}

void
AnotherScene::handleEvents(const Event& event)
{
    if (event.type() == EventType::KEYBOARD) {
        KeyboardEvent kb = event.keyboard();
        if (kb.action == KeyAction::PRESSED && !kb.repeat) {
            if (kb.key == Key::ESCAPE)
                manager->pop();
            else if (kb.key == Key::L)
                manager->changeScene(std::make_unique<YetAgainScene>(renderer));
            else if (kb.key == Key::J)
                manager->changeScene(std::make_unique<MainScene>(renderer));
        }
    }
}

void
AnotherScene::render(float alpha)
{
    (void)alpha;
    text.draw();
    info_text.draw();
}
