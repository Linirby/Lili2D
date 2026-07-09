#include "app.hpp"

App::App() : lili::Game("hello_animation - Lili2D", 400, 400) {
	setTps(20.0f);
	window->setResizable(true);

	head_atlas = lili::AtlasMap(renderer.get(), "head_animation.png");
	head_atlas.slice(8, 1);
	head_sprite = lili::AnimatedSprite(
		renderer.get(), lili::Animation(head_atlas.getSliceUVs(0, 8))
	);
	head_sprite.setScale({ 15, 15 });
	head_sprite.setFrameSpeed(0.2f);
}

void App::onUpdate(float dt) {
	head_sprite.setPosition(lili::Vec2(
		(float)window->getWidth() / 2 - head_sprite.getWidth() / 2,
		(float)window->getHeight() - head_sprite.getHeight()
	));
	head_sprite.update(dt);
}

void App::onRender(float alpha) {
	(void)alpha;
	head_sprite.draw();
}
