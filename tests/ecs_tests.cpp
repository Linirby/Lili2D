#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <string>
#include <vector>

#include "lili2d/ecs.hpp"

using namespace lili;

struct Position {
    float x = 0.0f;
    float y = 0.0f;
};

struct Velocity {
    float vx = 0.0f;
    float vy = 0.0f;
};

struct Name {
    std::string value;
};

TEST_CASE("ECS Registry - Entity Lifecycle", "[ecs][entity]") {
    ECSRegistry registry;

    Entity e1 = registry.createEntity();
    CHECK(registry.isValid(e1));
    CHECK(getEntityID(e1) == 0);
    CHECK(getEntityGen(e1) == 0);

    Entity e2 = registry.createEntity();
    CHECK(registry.isValid(e2));
    CHECK(getEntityID(e2) == 1);

    registry.destroyEntity(e1);
    CHECK_FALSE(registry.isValid(e1));
    CHECK(registry.isValid(e2));

    // Recycled entity ID with incremented version/generation
    Entity e3 = registry.createEntity();
    CHECK(registry.isValid(e3));
    CHECK(getEntityID(e3) == 0);
    CHECK(getEntityGen(e3) == 1);
    CHECK_FALSE(registry.isValid(e1));
}

TEST_CASE("ECS Registry - Components", "[ecs][components]") {
    ECSRegistry registry;
    Entity e = registry.createEntity();

    SECTION("Emplace and Query") {
        CHECK_FALSE(registry.hasComponent<Position>(e));
        CHECK_FALSE(registry.hasComponent<Velocity>(e));

        registry.emplaceComponent<Position>(e, 10.0f, 20.0f);
        CHECK(registry.hasComponent<Position>(e));
        CHECK_FALSE(registry.hasComponent<Velocity>(e));

        Position& pos = registry.getComponent<Position>(e);
        CHECK(pos.x == 10.0f);
        CHECK(pos.y == 20.0f);

        pos.x = 42.0f;
        CHECK(registry.getComponent<Position>(e).x == 42.0f);
    }

    SECTION("Remove Component") {
        registry.emplaceComponent<Position>(e, 1.0f, 2.0f);
        registry.emplaceComponent<Velocity>(e, 3.0f, 4.0f);

        CHECK(registry.hasComponent<Position>(e));
        CHECK(registry.hasComponent<Velocity>(e));

        registry.removeComponent<Position>(e);
        CHECK_FALSE(registry.hasComponent<Position>(e));
        CHECK(registry.hasComponent<Velocity>(e));
    }

    SECTION("Destroy Entity Clears Components") {
        registry.emplaceComponent<Position>(e, 5.0f, 5.0f);
        registry.destroyEntity(e);

        Entity new_e = registry.createEntity();
        CHECK_FALSE(registry.hasComponent<Position>(new_e));
    }
}

TEST_CASE("ECS Component Pool - Contiguous Iteration", "[ecs][pool]") {
    ECSRegistry registry;
    Entity e1 = registry.createEntity();
    Entity e2 = registry.createEntity();
    Entity e3 = registry.createEntity();

    registry.emplaceComponent<Position>(e1, 1.0f, 1.0f);
    registry.emplaceComponent<Position>(e2, 2.0f, 2.0f);
    registry.emplaceComponent<Position>(e3, 3.0f, 3.0f);

    auto& pool = registry.getPool<Position>();
    CHECK(pool.size() == 3);

    float sum_x = 0.0f;
    for (const auto& comp : pool.getComponents()) {
        sum_x += comp.x;
    }
    CHECK(sum_x == 6.0f);

    // Remove middle component to verify swap-and-pop integrity
    registry.removeComponent<Position>(e2);
    CHECK(pool.size() == 2);
    CHECK(registry.hasComponent<Position>(e1));
    CHECK_FALSE(registry.hasComponent<Position>(e2));
    CHECK(registry.hasComponent<Position>(e3));
    CHECK(registry.getComponent<Position>(e3).x == 3.0f);
}

TEST_CASE("ECS CommandBuffer - Deferred Operations", "[ecs][command_buffer]") {
    ECSRegistry registry;
    Entity e = registry.createEntity();

    CommandBuffer cmd;
    cmd.emplaceComponent<Position>(e, 100.0f, 200.0f);
    cmd.emplaceComponent<Name>(e, "Hero");

    // Before play, registry component does not exist
    CHECK_FALSE(registry.hasComponent<Position>(e));

    cmd.play(registry);

    // After play, components are emplaced
    CHECK(registry.hasComponent<Position>(e));
    CHECK(registry.getComponent<Position>(e).x == 100.0f);
    CHECK(registry.hasComponent<Name>(e));
    CHECK(registry.getComponent<Name>(e).value == "Hero");

    cmd.destroyEntity(e);
    CHECK(registry.isValid(e));
    cmd.play(registry);
    CHECK_FALSE(registry.isValid(e));
}

TEST_CASE("ECS View - Multi-Component Iteration", "[ecs][view]") {
    ECSRegistry registry;

    Entity e1 = registry.createEntity();
    Entity e2 = registry.createEntity();
    Entity e3 = registry.createEntity();
    Entity e4 = registry.createEntity();

    // e1: Position + Velocity
    registry.emplaceComponent<Position>(e1, 1.0f, 2.0f);
    registry.emplaceComponent<Velocity>(e1, 10.0f, 20.0f);

    // e2: Position only
    registry.emplaceComponent<Position>(e2, 3.0f, 4.0f);

    // e3: Velocity only
    registry.emplaceComponent<Velocity>(e3, 30.0f, 40.0f);

    // e4: Position + Velocity + Name
    registry.emplaceComponent<Position>(e4, 5.0f, 6.0f);
    registry.emplaceComponent<Velocity>(e4, 50.0f, 60.0f);
    registry.emplaceComponent<Name>(e4, "Player");

    SECTION("Empty View") {
        struct UnregisteredComponent {
            int dummy;
        };
        auto empty_view = registry.view<UnregisteredComponent>();
        CHECK(empty_view.empty());
        CHECK(empty_view.begin() == empty_view.end());

        auto mixed_empty_view =
            registry.view<Position, UnregisteredComponent>();
        CHECK(mixed_empty_view.empty());
        CHECK(mixed_empty_view.begin() == mixed_empty_view.end());
    }

    SECTION("Single Component View") {
        auto pos_view = registry.view<Position>();
        CHECK_FALSE(pos_view.empty());

        int count = 0;
        float total_x = 0.0f;
        for (auto [e, pos] : pos_view) {
            count++;
            total_x += pos.x;
        }
        CHECK(count == 3);  // e1, e2, e4
        CHECK(total_x == 1.0f + 3.0f + 5.0f);
    }

    SECTION("Multi-Component Filtering") {
        auto pv_view = registry.view<Position, Velocity>();
        CHECK_FALSE(pv_view.empty());

        std::vector<Entity> matched_entities;
        for (auto [e, pos, vel] : pv_view) {
            matched_entities.push_back(e);
            CHECK(registry.getComponent<Position>(e).x == pos.x);
            CHECK(registry.getComponent<Velocity>(e).vx == vel.vx);
        }

        CHECK(matched_entities.size() == 2);
        CHECK(
            std::find(
                matched_entities.begin(), matched_entities.end(), e1
            ) != matched_entities.end()
        );
        CHECK(
            std::find(
                matched_entities.begin(), matched_entities.end(), e4
            ) != matched_entities.end()
        );
        CHECK(
            std::find(
                matched_entities.begin(), matched_entities.end(), e2
            ) == matched_entities.end()
        );
        CHECK(
            std::find(
                matched_entities.begin(), matched_entities.end(), e3
            ) == matched_entities.end()
        );
    }

    SECTION("Three Component View") {
        auto all_view = registry.view<Position, Velocity, Name>();
        int count = 0;
        for (auto [e, pos, vel, name] : all_view) {
            CHECK(e == e4);
            CHECK(name.value == "Player");
            count++;
        }
        CHECK(count == 1);
    }

    SECTION("In-Place Mutation via View") {
        for (auto [e, pos, vel] : registry.view<Position, Velocity>()) {
            pos.x += vel.vx;
            pos.y += vel.vy;
        }

        CHECK(registry.getComponent<Position>(e1).x == 11.0f);
        CHECK(registry.getComponent<Position>(e1).y == 22.0f);
        CHECK(registry.getComponent<Position>(e4).x == 55.0f);
        CHECK(registry.getComponent<Position>(e4).y == 66.0f);
        // e2 has no Velocity, so its Position was not modified
        CHECK(registry.getComponent<Position>(e2).x == 3.0f);
        CHECK(registry.getComponent<Position>(e2).y == 4.0f);
    }

    SECTION("View Reacts to Component Removal and Entity Destruction") {
        registry.removeComponent<Velocity>(e1);

        int count = 0;
        for (auto [e, pos, vel] : registry.view<Position, Velocity>()) {
            CHECK(e == e4);
            count++;
        }
        CHECK(count == 1);

        registry.destroyEntity(e4);
        CHECK(registry.view<Position, Velocity>().empty());
    }

    SECTION("Const View Usage") {
        const auto const_view = registry.view<Position, Velocity>();
        CHECK_FALSE(const_view.empty());
        int count = 0;
        for (auto [e, pos, vel] : const_view) {
            (void)e;
            (void)pos;
            (void)vel;
            count++;
        }
        CHECK(count == 2);
    }
}

