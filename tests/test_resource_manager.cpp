#include <cassert>
#include <chrono>
#include <fstream>
#include <iostream>
#include <thread>

#include "lili2d/core/asset_manager.hpp"
#include "lili2d/core/resource_manager.hpp"

struct DummyResource {
    std::string name;
    int version = 1;
};

struct AudioResource {
    std::string title;
    float duration = 0.0f;
};

void test_deduplication() {
    lili::ResourceManager<DummyResource> manager;

    auto loader = [](const std::string& path) {
        auto res = std::make_unique<DummyResource>();
        res->name = path;
        return res;
    };

    DummyResource* res1 = manager.load("player", "assets/player.png", loader, "global");
    DummyResource* res2 = manager.load("player", "assets/player.png", loader, "global");
    DummyResource* res3 = manager.get("player");

    assert(res1 != nullptr);
    assert(res1 == res2);
    assert(res1 == res3);
    assert(res1->name == "assets/player.png");
    assert(manager.count() == 1);

    std::cout << "[PASS] Resource deduplication test\n";
}

void test_scope_unloading() {
    lili::ResourceManager<DummyResource> manager;

    auto loader = [](const std::string& path) {
        auto res = std::make_unique<DummyResource>();
        res->name = path;
        return res;
    };

    manager.load("bg_global", "assets/bg.png", loader, "global");
    manager.load("ui_button", "assets/button.png", loader, "menu");
    manager.load("ui_title", "assets/title.png", loader, "menu");
    manager.load("enemy", "assets/enemy.png", loader, "gameplay");

    assert(manager.count() == 4);

    size_t unloaded_menu = manager.unloadScope("menu");
    assert(unloaded_menu == 2);
    assert(manager.count() == 2);
    assert(manager.get("ui_button") == nullptr);
    assert(manager.get("ui_title") == nullptr);
    assert(manager.get("bg_global") != nullptr);
    assert(manager.get("enemy") != nullptr);

    manager.clear();
    assert(manager.count() == 0);

    std::cout << "[PASS] Scope unloading test\n";
}

void test_emplace_and_getref() {
    lili::ResourceManager<DummyResource> manager;

    auto item = std::make_unique<DummyResource>();
    item->name = "custom_item";
    item->version = 42;

    DummyResource* ptr = manager.emplace("custom_key", std::move(item), "level1");
    assert(ptr != nullptr);
    assert(manager.has("custom_key"));
    assert(manager.getRef("custom_key").version == 42);

    std::cout << "[PASS] Emplace & getRef test\n";
}

void test_hot_reloading() {
    lili::ResourceManager<DummyResource> manager;

    std::string temp_file = "temp_dummy_asset.txt";

    // Create temp file
    {
        std::ofstream out(temp_file);
        out << "version 1";
    }

    int reload_count = 0;

    auto loader = [&reload_count](const std::string& path) {
        auto res = std::make_unique<DummyResource>();
        std::ifstream in(path);
        std::string text;
        if (in >> text >> res->version) {
            // loaded successfully
        } else {
            res->version = 1;
        }
        return res;
    };

    auto reloader = [&reload_count](DummyResource& res, const std::string& path) {
        std::ifstream in(path);
        std::string text;
        if (in >> text >> res.version) {
            // reloaded successfully
        }
        reload_count++;
        return true;
    };

    DummyResource* res = manager.load("temp_asset", temp_file, loader, "dev", reloader);
    assert(res != nullptr);
    assert(res->version == 1);

    manager.setHotReloadEnabled(true);
    assert(manager.isHotReloadEnabled());

    // Wait a bit to ensure filesystem timestamp increases
    std::this_thread::sleep_for(std::chrono::milliseconds(1100));

    // Update temp file
    {
        std::ofstream out(temp_file);
        out << "version 2";
    }

    manager.checkHotReload();

    assert(reload_count == 1);
    assert(res->version == 2);

    // Clean up temp file
    std::remove(temp_file.c_str());

    std::cout << "[PASS] Hot reloading test\n";
}

void test_generic_type_manager() {
    lili::Assets::clear();

    auto& audio_mgr = lili::Assets::getManager<AudioResource>();
    auto loader = [](const std::string& path) {
        auto res = std::make_unique<AudioResource>();
        res->title = path;
        res->duration = 3.5f;
        return res;
    };

    audio_mgr.load("bgm_menu", "audio/menu.wav", loader, "menu");
    audio_mgr.load("bgm_level1", "audio/level1.wav", loader, "level1");

    assert(audio_mgr.count() == 2);
    assert(audio_mgr.has("bgm_menu"));

    // Verify polymorphic scope unloading across custom type managers
    size_t unloaded = lili::Assets::unloadScope("menu");
    assert(unloaded == 1);
    assert(!audio_mgr.has("bgm_menu"));
    assert(audio_mgr.has("bgm_level1"));

    // Verify polymorphic clearing
    lili::Assets::clear();
    assert(audio_mgr.count() == 0);

    std::cout << "[PASS] Generic type manager & polymorphic clear test\n";
}

int main() {
    std::cout << "Running Lili2D ResourceManager unit tests...\n";

    test_deduplication();
    test_scope_unloading();
    test_emplace_and_getref();
    test_hot_reloading();
    test_generic_type_manager();

    std::cout << "All ResourceManager tests passed successfully!\n";
    return 0;
}
