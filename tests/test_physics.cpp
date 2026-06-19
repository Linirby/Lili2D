#include <catch2/catch_test_macros.hpp>
#include "lili2d/physics/collision.hpp"
#include "lili2d/geometry/vec2.hpp"

using namespace lili;

TEST_CASE("AABB Intersection and Bounds", "[physics][aabb]") {
    SECTION("Basic Intersection") {
        AABB box1(Vec2(0.0f, 0.0f), Vec2(10.0f, 10.0f));
        AABB box2(Vec2(5.0f, 5.0f), Vec2(10.0f, 10.0f));
        AABB box3(Vec2(20.0f, 20.0f), Vec2(10.0f, 10.0f));

        REQUIRE(box1.intersect(box2) == true);
        REQUIRE(box2.intersect(box1) == true);
        REQUIRE(box1.intersect(box3) == false);
    }

    SECTION("Contains Logic") {
        AABB outer(Vec2(0.0f, 0.0f), Vec2(20.0f, 20.0f));
        AABB inner(Vec2(5.0f, 5.0f), Vec2(10.0f, 10.0f));
        AABB partial(Vec2(15.0f, 15.0f), Vec2(10.0f, 10.0f));

        REQUIRE(outer.contains(inner) == true);
        REQUIRE(inner.contains(outer) == false);
        REQUIRE(outer.contains(partial) == false);
    }
}
