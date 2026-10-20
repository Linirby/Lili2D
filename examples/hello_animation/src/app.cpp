#include "app.hpp"

using namespace lili;

App::App()
  : Game("hello_animation - Lili2D", 400, 400)
{
    setTps(20.0f);
    getWindow()->setResizable(true);
    Assets::setHotReloadEnabled(true);

    Renderer* renderer = getRenderer();

    AtlasMap* head_atlas =
        Assets::loadAtlas("head_atlas", renderer, "head_animation.png", 8, 1);

    head_sprite =
        AnimatedSprite(renderer, Animation(head_atlas->getSliceUVs(0, 8)));
    head_sprite.setScale({ 15.0f, 15.0f });
    head_sprite.setFrameSpeed(0.2f);
    head_sprite.setRender(RenderLayer::UI);
    head_sprite.setAnchor(Anchor::BOTTOM);
    head_sprite.setPivot(Pivot::BOTTOM);
}

void
App::onEvent(const Event& event)
{
    Game::onEvent(event);

    if (event.type() == EventType::KEYBOARD) {
        KeyboardEvent kb = event.keyboard();
        if (event.keyJustPressed(kb) && kb.key == Key::ESCAPE)
            shutdown();
    }
}

void
App::onUpdate(float dt)
{
    Game::onUpdate(dt);

    head_sprite.update(dt);
}

void
App::onRender(float alpha)
{
    Game::onRender(alpha);

    head_sprite.draw();
}
