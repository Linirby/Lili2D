#include "another_menu.hpp"

#include "main_menu.hpp"
#include "yet_again_menu.hpp"

AnotherScene::AnotherScene(lili::Renderer *renderer)
	: lili::Scene("AnotherMenu", renderer),
	font(renderer, "lili_font.png", 16, 6),
	text(renderer, &font, "AnotherMenu Scene"),
	info_text(renderer, &font, "J and L to change") {
	text.setScale(3.0f);
	info_text.setPosition({0, 30});
	info_text.setScale(2.0f);
}

void AnotherScene::handleEvents(const lili::Event &event) {
	lili::KeyboardEvent keyboard = event.keyboard();
	if (keyboard.action == lili::KeyAction::PRESSED && !keyboard.repeat) {
		if (keyboard.key == SDLK_ESCAPE)
			manager->pop();
		if (keyboard.key == SDLK_L)
			manager->changeScene(std::make_unique<YetAgainScene>(renderer));
		if (keyboard.key == SDLK_J)
			manager->changeScene(std::make_unique<MainScene>(renderer));
	}
}

void AnotherScene::render(float alpha) {
	(void)alpha;
	text.draw();
	info_text.draw();
}
