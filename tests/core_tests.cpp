#include <atomic>
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <chrono>
#include <string>
#include <thread>

#include "lili2d/core.hpp"

using namespace lili;

TEST_CASE("StringHash and StringMap", "[core][string_hash]") {
    StringMap<int> map;
    map["player"] = 100;
    map["enemy"] = 50;

    std::string_view key = "player";
    auto it = map.find(key);
    REQUIRE(it != map.end());
    CHECK(it->second == 100);

    CHECK(map.contains("enemy"));
    CHECK_FALSE(map.contains("boss"));
}

TEST_CASE("Clock TPS and Accumulator", "[core][clock]") {
    Clock clock(60.0f);
    CHECK(clock.getTps() == 60.0f);
    CHECK(clock.getFixedDt() == 1.0f / 60.0f);

    clock.setTps(30.0f);
    CHECK(clock.getTps() == 30.0f);
    CHECK(clock.getFixedDt() == 1.0f / 30.0f);
}

TEST_CASE("ActionMap Key and Mouse Bindings", "[core][action_map]") {
    ActionMap& action_map = ActionMap::get();
    action_map.clear();

    action_map.add("Jump", {Key::SPACE, Key::W});
    CHECK(action_map.has("Jump"));
    CHECK(action_map.getKeys("Jump").size() == 2);

    action_map.add("Fire", {}, {MouseButton::LEFT});
    CHECK(action_map.has("Fire"));
    REQUIRE(action_map.getMouseButtons("Fire").size() == 1);
    CHECK(action_map.getMouseButtons("Fire")[0] == MouseButton::LEFT);

    action_map.removeKey("Jump", Key::W);
    REQUIRE(action_map.getKeys("Jump").size() == 1);
    CHECK(action_map.getKeys("Jump")[0] == Key::SPACE);

    action_map.remove("Jump");
    CHECK_FALSE(action_map.has("Jump"));
    action_map.clear();
}

TEST_CASE("ThreadPool Execution", "[core][thread_pool]") {
    EngineConfig config;
    config.thread_count_override = 4;
    ThreadPool pool(config);

    std::atomic<int> counter{0};
    constexpr int TASK_COUNT = 50;

    for (int i = 0; i < TASK_COUNT; ++i) {
        pool.enqueue(
            [&counter]() { counter.fetch_add(1, std::memory_order_relaxed); },
            TaskPriority::NORMAL
        );
    }

    // Wait briefly for all background tasks to complete
    auto start = std::chrono::steady_clock::now();
    while (counter.load(std::memory_order_relaxed) < TASK_COUNT) {
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        if (std::chrono::steady_clock::now() - start >
            std::chrono::seconds(2)) {
            break;
        }
    }

    CHECK(counter.load() == TASK_COUNT);
}

struct DummyResource {
    std::string tag;
};

TEST_CASE("ResourceManager Generic Cache", "[core][resource_manager]") {
    ResourceManager<DummyResource> manager;

    auto dummy_loader = [](const std::string& path) {
        auto res = std::make_unique<DummyResource>();
        res->tag = "loaded:" + path;
        return res;
    };

    DummyResource* r1 =
        manager.load("hero", "assets/hero.png", dummy_loader, "level1");
    REQUIRE(r1 != nullptr);
    CHECK(r1->tag == "loaded:assets/hero.png");
    CHECK(manager.count() == 1);
    CHECK(manager.has("hero"));

    // Cache hit: pointer should be identical
    DummyResource* r2 = manager.get("hero");
    CHECK(r1 == r2);

    manager.emplace(
        "custom", std::make_unique<DummyResource>(DummyResource{"custom_val"}),
        "ui"
    );
    CHECK(manager.count() == 2);
    CHECK(manager.getRef("custom").tag == "custom_val");

    // Unload by scope
    size_t unloaded = manager.unloadScope("level1");
    CHECK(unloaded == 1);
    CHECK_FALSE(manager.has("hero"));
    CHECK(manager.has("custom"));

    manager.clear();
    CHECK(manager.count() == 0);
}

TEST_CASE("Easing Evaluation", "[core][easing]") {
    CHECK(Easing::evaluate(EaseType::LINEAR, 0.0f) == 0.0f);
    CHECK(Easing::evaluate(EaseType::LINEAR, 1.0f) == 1.0f);
    CHECK(Easing::evaluate(EaseType::LINEAR, 0.5f) == 0.5f);

    // Boundary conditions clamped
    CHECK(Easing::evaluate(EaseType::IN_QUAD, -0.5f) == 0.0f);
    CHECK(Easing::evaluate(EaseType::IN_QUAD, 1.5f) == 1.0f);

    // Quadratic
    CHECK(Easing::evaluate(EaseType::IN_QUAD, 0.5f) == 0.25f);
    CHECK(Easing::evaluate(EaseType::OUT_QUAD, 0.5f) == 0.75f);
    CHECK(Easing::evaluate(EaseType::IN_OUT_QUAD, 0.5f) == 0.5f);

    // Cubic
    CHECK(Easing::evaluate(EaseType::IN_CUBIC, 0.5f) == 0.125f);
    CHECK(Easing::evaluate(EaseType::OUT_CUBIC, 0.5f) == 0.875f);
    CHECK(Easing::evaluate(EaseType::IN_OUT_CUBIC, 0.5f) == 0.5f);

    // Midpoints for symmetric InOut curves
    CHECK(Easing::evaluate(EaseType::IN_OUT_SINE, 0.5f) == Catch::Approx(0.5f));
    CHECK(Easing::evaluate(EaseType::IN_OUT_EXPO, 0.5f) == Catch::Approx(0.5f));
    CHECK(Easing::evaluate(EaseType::IN_OUT_BACK, 0.5f) == Catch::Approx(0.5f));
    CHECK(
        Easing::evaluate(EaseType::IN_OUT_BOUNCE, 0.5f) == Catch::Approx(0.5f)
    );

    // Direct static method calls
    CHECK(Easing::linear(0.5f) == 0.5f);
    CHECK(Easing::inQuad(0.5f) == 0.25f);
    CHECK(Easing::outQuad(0.5f) == 0.75f);
    CHECK(Easing::inOutQuad(0.5f) == 0.5f);
    CHECK(Easing::inCubic(0.5f) == 0.125f);
    CHECK(Easing::outCubic(0.5f) == 0.875f);
    CHECK(Easing::inOutCubic(0.5f) == 0.5f);
    CHECK(Easing::inSine(0.0f) == 0.0f);
    CHECK(Easing::outSine(1.0f) == 1.0f);
    CHECK(Easing::outBounce(0.0f) == 0.0f);
    CHECK(Easing::outBounce(1.0f) == 1.0f);
    CHECK(Easing::inBounce(0.0f) == 0.0f);
    CHECK(Easing::inBounce(1.0f) == 1.0f);
}
