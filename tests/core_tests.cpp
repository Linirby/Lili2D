#include <atomic>
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <string>
#include <thread>

#include "lili2d/core.hpp"

using namespace lili;

TEST_CASE("StringHash and StringMap", "[core][string_hash]")
{
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

TEST_CASE("Clock TPS and Accumulator", "[core][clock]")
{
    Clock clock(60.0f);
    CHECK(clock.getTps() == 60.0f);
    CHECK(clock.getFixedDt() == 1.0f / 60.0f);

    clock.setTps(30.0f);
    CHECK(clock.getTps() == 30.0f);
    CHECK(clock.getFixedDt() == 1.0f / 30.0f);
}

TEST_CASE("Clock FPS Limiter", "[core][clock]")
{
    Clock clock(60.0f);
    CHECK(clock.getMaxFps() == 0);

    clock.setMaxFps(120);
    CHECK(clock.getMaxFps() == 120);

    clock.setMaxFps(0);
    CHECK(clock.getMaxFps() == 0);

    Clock capped_clock(60.0f, 60);
    CHECK(capped_clock.getMaxFps() == 60);

    // Test precision limiting with a 200 FPS cap (5ms per frame)
    capped_clock.setMaxFps(200);
    capped_clock.reset();

    auto start = std::chrono::steady_clock::now();
    capped_clock.update();
    capped_clock.limitFps();
    auto elapsed_us = std::chrono::duration_cast<std::chrono::microseconds>(
                          std::chrono::steady_clock::now() - start
    )
                          .count();
    CHECK(elapsed_us >= 4000);
}

TEST_CASE("GameConfig Max FPS", "[core][config]")
{
    GameConfig& config = GameConfig::get();
    config.updateMaxFps(60);
    CHECK(config.getMaxFps() == 60);
    config.updateMaxFps(0);
    CHECK(config.getMaxFps() == 0);
}

TEST_CASE("ActionMap Key and Mouse Bindings", "[core][action_map]")
{
    ActionMap& action_map = ActionMap::get();
    action_map.clear();

    action_map.add("Jump", { Key::SPACE, Key::W });
    CHECK(action_map.has("Jump"));
    CHECK(action_map.getKeys("Jump").size() == 2);

    action_map.add("Fire", {}, { MouseButton::LEFT });
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

TEST_CASE("ThreadPool Execution", "[core][thread_pool]")
{
    EngineConfig config;
    config.thread_count_override = 4;
    ThreadPool pool(config);

    std::atomic<int> counter{ 0 };
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

struct DummyResource
{
    std::string tag;
};

TEST_CASE("ResourceManager Generic Cache", "[core][resource_manager]")
{
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
        "custom",
        std::make_unique<DummyResource>(DummyResource{ "custom_val" }),
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

TEST_CASE(
    "ResourceManager Hot Reload Single Failure Notice",
    "[core][resource_manager]"
)
{
    std::filesystem::path temp_file =
        std::filesystem::temp_directory_path() / "lili2d_test_reload.txt";
    {
        std::ofstream out(temp_file);
        out << "version 1";
    }

    ResourceManager<DummyResource> manager;
    manager.setHotReloadEnabled(true);

    int reloader_calls = 0;
    auto loader = [](const std::string& path) {
        auto res = std::make_unique<DummyResource>();
        res->tag = "loaded:" + path;
        return res;
    };
    auto failing_reloader =
        [&reloader_calls](DummyResource&, const std::string&) -> bool {
        ++reloader_calls;
        return false;
    };

    manager.load(
        "test_res", temp_file.string(), loader, "test", failing_reloader
    );
    CHECK(reloader_calls == 0);

    // Sleep briefly to ensure new timestamp on filesystem
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    {
        std::ofstream out(temp_file);
        out << "version 2 with error";
    }

    // First check: should attempt reload once
    manager.checkHotReload();
    CHECK(reloader_calls == 1);

    // Second check without file change: should NOT attempt reload again
    manager.checkHotReload();
    CHECK(reloader_calls == 1);

    std::filesystem::remove(temp_file);
}

TEST_CASE("Easing Evaluation", "[core][easing]")
{
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

TEST_CASE("Timer and TimerManager", "[core][timer]")
{
    SECTION("Timer Progress and Expiration")
    {
        Timer timer(1.0f, false, true);
        CHECK(timer.isRunning());
        CHECK_FALSE(timer.isFinished());
        CHECK(timer.getProgress() == 0.0f);

        timer.update(0.5f);
        CHECK(timer.getElapsed() == 0.5f);
        CHECK(timer.getProgress() == 0.5f);
        CHECK(timer.getRemaining() == 0.5f);
        CHECK_FALSE(timer.isFinished());

        bool completed = false;
        timer.onComplete([&]() { completed = true; });

        timer.update(0.5f);
        CHECK(completed);
        CHECK(timer.isFinished());
        CHECK_FALSE(timer.isRunning());
    }

    SECTION("Repeating Timer")
    {
        Timer timer(1.0f, true, true);
        int completions = 0;
        timer.onComplete([&]() { completions++; });

        timer.update(1.0f);
        CHECK(completions == 1);
        CHECK_FALSE(timer.isFinished());
        CHECK(timer.isRunning());

        timer.update(1.0f);
        CHECK(completions == 2);
    }

    SECTION("TimerManager Lifecycle")
    {
        TimerManager manager;
        bool t1_done = false;
        bool t2_done = false;

        manager.create(0.5f, [&]() { t1_done = true; });
        manager.create(1.0f, [&]() { t2_done = true; });

        CHECK(manager.count() == 2);

        manager.update(0.6f);
        CHECK(t1_done);
        CHECK_FALSE(t2_done);
        CHECK(manager.count() == 1);

        manager.update(0.5f);
        CHECK(t2_done);
        CHECK(manager.count() == 0);
        CHECK(manager.empty());
    }
}

TEST_CASE("Type Traits and Move Guarantees", "[core][traits]")
{
    static_assert(std::is_nothrow_move_constructible_v<Clock>);
    static_assert(std::is_nothrow_move_assignable_v<Clock>);

    static_assert(std::is_nothrow_move_constructible_v<TimerManager>);
    static_assert(std::is_nothrow_move_assignable_v<TimerManager>);

    // ThreadPool is pinned with active jthreads, non-movable and non-copyable
    static_assert(!std::is_move_constructible_v<ThreadPool>);
    static_assert(!std::is_move_assignable_v<ThreadPool>);
    static_assert(!std::is_copy_constructible_v<ThreadPool>);
    static_assert(!std::is_copy_assignable_v<ThreadPool>);

    static_assert(std::is_nothrow_move_constructible_v<Window>);
    static_assert(std::is_nothrow_move_assignable_v<Window>);

    CHECK(true);
}

#include "lili2d/render/default_font.hpp"
#include "lili2d/render/scene/common/text.hpp"

TEST_CASE("Default Font Asset Integrity", "[render][font]")
{
    CHECK(default_font_cols == 16);
    CHECK(default_font_rows == 6);
    REQUIRE(default_font_png_len > 8);
    // PNG signature: 0x89 0x50 0x4E 0x47 0x0D 0x0A 0x1A 0x0A
    CHECK(default_font_png[0] == 0x89);
    CHECK(default_font_png[1] == 'P');
    CHECK(default_font_png[2] == 'N');
    CHECK(default_font_png[3] == 'G');
    CHECK(default_font_png[4] == 0x0D);
    CHECK(default_font_png[5] == 0x0A);
    CHECK(default_font_png[6] == 0x1A);
    CHECK(default_font_png[7] == 0x0A);

    static_assert(std::is_nothrow_move_constructible_v<BitmapFont>);
    static_assert(std::is_nothrow_move_assignable_v<BitmapFont>);
    static_assert(std::is_nothrow_move_constructible_v<Text>);
    static_assert(std::is_nothrow_move_assignable_v<Text>);
}
