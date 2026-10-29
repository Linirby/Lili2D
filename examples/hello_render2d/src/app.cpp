#include "app.hpp"

#include "rng_helper.hpp"

using namespace lili;

void
App::addBaguette(ECSRegistry& registry, Renderer* renderer, Vec2 pos)
{
    Entity e = createSprite(
        registry,
        renderer,
        "baguette.jpg",
        { .pos = pos, .scale = Vec2(0.5f, 0.5f) }
    );
    spawned.push_back(e);
}

void
App::addRect(
    ECSRegistry& registry,
    Vec2 pos,
    Vec2 size,
    Vec4 color,
    bool hollow,
    float thickness
)
{
    Entity e = createRect(
        registry,
        { .color = color,
          .pos = pos,
          .size = size,
          .scale = Vec2(1.0f, 1.0f),
          .rotation = 0.0f,
          .thickness = thickness,
          .hollow = hollow }
    );
    spawned.push_back(e);
}

void
App::addCircle(
    ECSRegistry& registry,
    Vec2 pos,
    float radius,
    Vec4 color,
    bool hollow,
    float thickness
)
{
    Entity e = createCircle(
        registry,
        {
            .transform{
                .pos = pos,
            },
            .render2d{
                .color = color,
            },
            .circle{
                .radius = radius,
                .thickness = thickness,
                .hollow = hollow,
            },
        }
    );
    spawned.push_back(e);
}

void
App::addLine(
    ECSRegistry& registry,
    Vec2 start_pos,
    Vec2 end_pos,
    float thickness,
    Vec4 color
)
{
    Entity e = createLine(
        registry,
        { .color = color,
          .start_pos = start_pos,
          .end_pos = end_pos,
          .thickness = thickness }
    );
    spawned.push_back(e);
}

void
App::popLast(ECSRegistry& registry)
{
    if (!spawned.empty()) {
        Entity last = spawned.back();
        spawned.pop_back();
        registry.destroyEntity(last);
    }
}

void
App::onInit()
{
    Renderer* renderer = getRenderer();

    instructions_text = Text(
        renderer,
        renderer->getDefaultFont(),
        "Key 1 to 4: Create renderable entity | Backspace: Remove last entity"
    );
    instructions_text.setScale(2.0f);
    instructions_text.setRender(RenderLayer::UI);
    instructions_text.setAnchor(Anchor::TOP);
    instructions_text.setPivot(Pivot::TOP);
    instructions_text.setPosition({ 0.0f, 10.0f });
}

void
App::onEvent(const Event& event)
{
    Game::onEvent(event);

    ECSRegistry& registry = getECSRegistry();
    Renderer* renderer = getRenderer();
    Vec2 window_size = getWindow()->getSize();

    if (event.type() == EventType::KEYBOARD) {
        KeyboardEvent kb_ev = event.keyboard();
        if (event.keyJustPressed(kb_ev)) {
            if (kb_ev.key == Key::ESCAPE)
                shutdown();
            else if (kb_ev.key == Key::NUM_1)
                addBaguette(registry, renderer, rng::pos(window_size));
            else if (kb_ev.key == Key::NUM_2)
                addRect(
                    registry,
                    rng::pos(window_size),
                    rng::size(),
                    rng::color(),
                    rng::hollow(),
                    rng::thickness()
                );
            else if (kb_ev.key == Key::NUM_3)
                addCircle(
                    registry,
                    rng::pos(window_size),
                    rng::radius(),
                    rng::color(),
                    rng::hollow(),
                    rng::thickness()
                );
            else if (kb_ev.key == Key::NUM_4)
                addLine(
                    registry,
                    rng::pos(window_size),
                    rng::pos(window_size),
                    rng::thickness(),
                    rng::color()
                );
            else if (kb_ev.key == Key::BACKSPACE)
                popLast(registry);
        }
    }
}

void
App::onRender(float alpha)
{
    Game::onRender(alpha);

    instructions_text.draw();
}
