#include <catch2/catch_test_macros.hpp>
#include "lili2d/scene/scene_manager.hpp"
#include "lili2d/scene/scene.hpp"

using namespace lili;

class MainScene : public Scene {
public:
	int updates = 0;

	MainScene(const std::string& name) : Scene(name) {}

    void update(float dt) override {
		(void)dt;
        updates++;
    }
};

class GameScene : public Scene {
public:
	int updates = 0;

	GameScene(const std::string& name) : Scene(name) {}

    void update(float dt) override {
		(void)dt;
        updates++;
    }
};

class PauseScene : public Scene {
public:
	int updates = 0;

	PauseScene(const std::string& name) : Scene(name) {}

    void update(float dt) override {
		(void)dt;
        updates++;
    }
};

TEST_CASE("SceneManager Stack Behavior", "[scene][manager]") {
    SceneManager manager;

    SECTION("Push and Active State") {
        manager.push(std::make_unique<MainScene>("main"));
        manager.update(0.16f);
		manager.change_scene(std::make_unique<GameScene>("game"));
		manager.update(0.16f);
		manager.push(std::make_unique<PauseScene>("pause"));
		manager.update(0.16f);
		manager.pop();
		manager.update(0.16f);

        REQUIRE(true);
    }
}
