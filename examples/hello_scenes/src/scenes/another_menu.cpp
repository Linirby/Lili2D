#include "another_menu.hpp"

#include "main_menu.hpp"
#include "yet_again_menu.hpp"

AnotherScene::AnotherScene(lili::Renderer* renderer)
    : lili::Scene("AnotherMenu", renderer),
      text(
          renderer,
          lili::Assets::loadFont("lili_font", renderer, "lili_font.png", 16, 6),
          "AnotherMenu Scene"
      ),
      info_text(
          renderer,
          lili::Assets::loadFont("lili_font", renderer, "lili_font.png", 16, 6),
          "J and L to change"
      ) {
    text.setScale(3.0f);
    text.setRender(lili::RenderLayer::UI);
    text.setAnchor(lili::Anchor::Center);
    text.setPivot(lili::Pivot::Bottom);
    info_text.setScale(2.0f);
    info_text.setRender(lili::RenderLayer::UI);
    info_text.setAnchor(lili::Anchor::Center);
    info_text.setPivot(lili::Pivot::Top);
}

void
AnotherScene::handleEvents(const lili::Event& event) {
    lili::KeyboardEvent keyboard = event.keyboard();
    if (keyboard.action == lili::KeyAction::PRESSED && !keyboard.repeat) {
        if (keyboard.key == SDLK_ESCAPE) manager->pop();
        if (keyboard.key == SDLK_L)
            manager->changeScene(std::make_unique<YetAgainScene>(renderer));
        if (keyboard.key == SDLK_J)
            manager->changeScene(std::make_unique<MainScene>(renderer));
    }
}

void
AnotherScene::render(float alpha) {
    (void)alpha;
    text.draw();
    info_text.draw();
}
