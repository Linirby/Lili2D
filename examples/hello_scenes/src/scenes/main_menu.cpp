#include "main_menu.hpp"

#include "another_menu.hpp"
#include "yet_again_menu.hpp"

MainScene::MainScene(lili::Renderer* renderer)
    : lili::Scene("MainMenu", renderer) {
    lili::BitmapFont* font =
        lili::Assets::loadFont("lili_font", renderer, "lili_font.png", 16, 6);

    text = lili::Text(renderer, font, "MainMenu Scene");
    text.setScale(3.0f);
    text.setRender(lili::RenderLayer::UI);
    text.setAnchor(lili::Anchor::CENTER);
    text.setPivot(lili::Pivot::BOTTOM);

    info_text = lili::Text(renderer, font, "J and L to change");
    info_text.setScale(2.0f);
    info_text.setRender(lili::RenderLayer::UI);
    info_text.setAnchor(lili::Anchor::CENTER);
    info_text.setPivot(lili::Pivot::TOP);
}

void
MainScene::handleEvents(const lili::Event& event) {
    if (event.type() == lili::EventType::KEYBOARD) {
        lili::KeyboardEvent kb = event.keyboard();
        if (kb.action == lili::KeyAction::PRESSED && !kb.repeat) {
            if (kb.key == lili::Key::ESCAPE)
                manager->pop();
            else if (kb.key == lili::Key::L)
                manager->changeScene(std::make_unique<AnotherScene>(renderer));
            else if (kb.key == lili::Key::J)
                manager->changeScene(std::make_unique<YetAgainScene>(renderer));
        }
    }
}

void
MainScene::render(float alpha) {
    (void)alpha;
    text.draw();
    info_text.draw();
}
