#include <catch2/catch_test_macros.hpp>
#include "lili2d/physics/collision.hpp"
#include "lili2d/geometry/vec2.hpp"

using namespace lili;

TEST_CASE("AABB2 Intersection and Bounds", "[physics][aabb]") {
    SECTION("Basic Intersection") {
        AABB2 box1(Vec2(0.0f, 0.0f), Vec2(10.0f, 10.0f));
        AABB2 box2(Vec2(5.0f, 5.0f), Vec2(10.0f, 10.0f));
        AABB2 box3(Vec2(20.0f, 20.0f), Vec2(10.0f, 10.0f));

        REQUIRE(box1.intersect(box2) == true);
        REQUIRE(box2.intersect(box1) == true);
        REQUIRE(box1.intersect(box3) == false);
    }

    SECTION("Contains Logic") {
        AABB2 outer(Vec2(0.0f, 0.0f), Vec2(20.0f, 20.0f));
        AABB2 inner(Vec2(5.0f, 5.0f), Vec2(10.0f, 10.0f));
        AABB2 partial(Vec2(15.0f, 15.0f), Vec2(10.0f, 10.0f));

        REQUIRE(outer.contains(inner) == true);
        REQUIRE(inner.contains(outer) == false);
        REQUIRE(outer.contains(partial) == false); // only partially overlaps
    }
}

TEST_CASE("AABB3 Intersection and Bounds", "[physics][aabb3]") {
    SECTION("Basic Intersection") {
        AABB3 box1(Vec3(0.0f, 0.0f, 0.0f), Vec3(10.0f, 10.0f, 10.0f));
        AABB3 box2(Vec3(5.0f, 5.0f, 5.0f), Vec3(10.0f, 10.0f, 10.0f));
        AABB3 box3(Vec3(20.0f, 20.0f, 20.0f), Vec3(10.0f, 10.0f, 10.0f));

        REQUIRE(box1.intersect(box2) == true);
        REQUIRE(box2.intersect(box1) == true);
        REQUIRE(box1.intersect(box3) == false);
    }

    SECTION("Contains Logic") {
        AABB3 outer(Vec3(0.0f, 0.0f, 0.0f), Vec3(20.0f, 20.0f, 20.0f));
        AABB3 inner(Vec3(5.0f, 5.0f, 5.0f), Vec3(10.0f, 10.0f, 10.0f));
        AABB3 partial(Vec3(15.0f, 15.0f, 15.0f), Vec3(10.0f, 10.0f, 10.0f));

        REQUIRE(outer.contains(inner) == true);
        REQUIRE(inner.contains(outer) == false);
        REQUIRE(outer.contains(partial) == false); // only partially overlaps
    }
}
