#include "yet_again_menu.hpp"

#include "main_menu.hpp"
#include "another_menu.hpp"

YetAgainScene::YetAgainScene(lili::Renderer *renderer)
	: lili::Scene("AnotherMenu", renderer),
	font(renderer, "lili_font.png", 16, 6),
	text(renderer, &font, "YetAgainMenu Scene"),
	info_text(renderer, &font, "J and L to change") {
	text.setScale(3.0f);
	info_text.setPosition({0, 30});
	info_text.setScale(2.0f);
}

void YetAgainScene::handleEvents(const lili::Event &event) {
	lili::KeyboardEvent keyboard = event.keyboard();
	if (keyboard.action == lili::KeyAction::PRESSED && !keyboard.repeat) {
		if (keyboard.key == SDLK_ESCAPE)
			manager->pop();
		if (keyboard.key == SDLK_L)
			manager->changeScene(std::make_unique<MainScene>(renderer));
		if (keyboard.key == SDLK_J)
			manager->changeScene(std::make_unique<AnotherScene>(renderer));
	}
}

void YetAgainScene::render(float alpha) {
	(void)alpha;
	text.draw();
	info_text.draw();
}
