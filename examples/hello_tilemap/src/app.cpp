#include "app.hpp"

#include <algorithm>
#include <cmath>

using namespace lili;

App::App()
  : Game("hello_tilemap - Lili2D", 768, 640)
{
    setTps(20.0f);
    Window* window = getWindow();
    Renderer* renderer = getRenderer();

    camera.setZoom(4.0f);
    camera.setPosition(
        { static_cast<float>(window->getWidth()),
          static_cast<float>(window->getHeight()) }
    );
    renderer->setCamera(&camera);

    Vec2 tile_render_size(16.0f, 16.0f);
    tilemap = std::make_unique<TileMap>(tile_render_size);

    env_atlas = Assets::loadAtlas(
        "env_atlas", renderer, "assets/environment.png", 3, 2
    );

    TileRegistry& registry = TileRegistry::get();
    registry.registerTile("grass:light", Tile(env_atlas->getSliceUV(0)));
    registry.registerTile("grass:medium", Tile(env_atlas->getSliceUV(1)));
    registry.registerTile("grass:dark", Tile(env_atlas->getSliceUV(2)));
    registry.registerTile("dirt:light", Tile(env_atlas->getSliceUV(3)));
    Tile invisible_solid;
    invisible_solid.is_solid = true;
    registry.registerTile("solid_invisible", std::move(invisible_solid));

    int map_width = 1500;
    int map_height = 1500;

    for (int y = 0; y < map_height; ++y) {
        for (int x = 0; x < map_width; ++x) {
            float noise = std::sin(x * 0.1f) * std::cos(y * 0.1f) * 3.0f;
            noise +=
                (std::sin(x * 0.05f + 10.0f) * 2.0f +
                 std::cos(y * 0.05f + 10.0f) * 2.0f);

            int elevation = static_cast<int>(noise + 3.0f);
            if (elevation < 0)
                elevation = 0;
            if (elevation > 5)
                elevation = 5;

            for (int z = 0; z < elevation; ++z)
                tilemap->setTile("solid_invisible", Point3(x, y, z));

            std::string tile_name;
            if (elevation >= 4)
                tile_name = "grass:light";
            else if (elevation >= 2)
                tile_name = "grass:medium";
            else
                tile_name = "grass:dark";

            tilemap->setTile(tile_name, Point3(x, y, elevation));
        }
    }

    BitmapFont* font =
        Assets::loadFont("lili_font", renderer, "assets/lili_font.png", 16, 6);
    text_infos = Text(renderer, font, "WASD: move | IK: zoom/dezoom");
    text_infos.setRender(RenderLayer::UI);
    text_infos.setPosition({ 10.0f, 10.0f });
    text_infos.setAnchor(Anchor::TOP);
    text_infos.setPivot(Pivot::TOP);
    text_infos.setScale(3.0f);
}

void
App::onEvent(const Event& event)
{
    Game::onEvent(event);
    if (event.type() == EventType::KEYBOARD) {
        KeyboardEvent kb = event.keyboard();
        if (kb.action == KeyAction::PRESSED && kb.key == Key::ESCAPE)
            shutdown();
    }
}

void
App::onUpdate(float dt)
{
    keyboard.update();
    Vec2 vel(0.0f, 0.0f);

    if (keyboard.held(Key::W))
        vel.y = -1.0f;
    if (keyboard.held(Key::S))
        vel.y = 1.0f;
    if (keyboard.held(Key::A))
        vel.x = -1.0f;
    if (keyboard.held(Key::D))
        vel.x = 1.0f;

    Vec2 camera_pos = camera.getPosition();
    camera.setPosition(camera_pos + (vel * 100.0f * dt));

    if (keyboard.held(Key::I))
        camera.setZoom(camera.getZoom() + dt);
    if (keyboard.held(Key::K))
        camera.setZoom(std::max(0.01f, camera.getZoom() - dt));
}

void
App::onRender([[maybe_unused]] float alpha)
{
    text_infos.draw();
    tilemap->draw(getRenderer(), getThreadPool());
}
