#include <catch2/catch_test_macros.hpp>

#include "lili2d/physics.hpp"

using namespace lili;

TEST_CASE("AABB2 Intersection and Containment", "[physics][aabb]") {
    AABB2 box1(Vec2(0.0f, 0.0f), Vec2(10.0f, 10.0f));
    AABB2 box2(Vec2(5.0f, 5.0f), Vec2(10.0f, 10.0f));
    AABB2 box3(Vec2(20.0f, 20.0f), Vec2(10.0f, 10.0f));
    AABB2 inner(Vec2(2.0f, 2.0f), Vec2(4.0f, 4.0f));

    SECTION("Intersect") {
        CHECK(box1.intersect(box2));
        CHECK(box2.intersect(box1));
        CHECK_FALSE(box1.intersect(box3));
        CHECK_FALSE(box3.intersect(box1));
    }

    SECTION("Contains") {
        CHECK(box1.contains(inner));
        CHECK_FALSE(inner.contains(box1));
        CHECK_FALSE(box1.contains(box2));
    }
}

TEST_CASE("CircleCollider Intersection and Containment", "[physics][circle]") {
    CircleCollider c1(Vec2(0.0f, 0.0f), 5.0f);
    CircleCollider c2(Vec2(6.0f, 0.0f), 3.0f);
    CircleCollider c3(Vec2(20.0f, 0.0f), 2.0f);
    CircleCollider inner(Vec2(1.0f, 0.0f), 2.0f);

    SECTION("Circle vs Circle Intersection") {
        CHECK(c1.intersect(c2));
        CHECK(c2.intersect(c1));
        CHECK_FALSE(c1.intersect(c3));
        CHECK_FALSE(c3.intersect(c1));
    }

    SECTION("Point Containment") {
        CHECK(c1.contains(Vec2(0.0f, 0.0f)));
        CHECK(c1.contains(Vec2(3.0f, 3.0f)));
        CHECK_FALSE(c1.contains(Vec2(5.1f, 0.0f)));
    }

    SECTION("Circle vs Circle Containment") {
        CHECK(c1.contains(inner));
        CHECK_FALSE(inner.contains(c1));
        CHECK_FALSE(c1.contains(c2));
    }

    SECTION("Circle vs AABB2 Intersection") {
        AABB2 box(Vec2(3.0f, -2.0f), Vec2(4.0f, 4.0f));
        CHECK(c1.intersect(box));

        AABB2 far_box(Vec2(10.0f, 10.0f), Vec2(2.0f, 2.0f));
        CHECK_FALSE(c1.intersect(far_box));
    }
}
