#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "lili2d/geometry/vec2.hpp"
#include "lili2d/geometry/vec3.hpp"
#include "lili2d/geometry/vec4.hpp"
#include "lili2d/geometry/point2.hpp"
#include "lili2d/geometry/point3.hpp"
#include "lili2d/geometry/mat3x3.hpp"
#include "lili2d/geometry/mat4x4.hpp"
#include "lili2d/geometry/utils.hpp"

using namespace lili;
using Catch::Matchers::WithinRel;
using Catch::Matchers::WithinAbs;

TEST_CASE("Vec2 Arithmetic", "[geometry][vec2]") {
    Vec2 a(1.0f, 2.0f);
    Vec2 b(3.0f, 4.0f);

    SECTION("Addition") {
        Vec2 c = a + b;
        REQUIRE_THAT(c.x, WithinRel(4.0f, 0.001f));
        REQUIRE_THAT(c.y, WithinRel(6.0f, 0.001f));
    }

    SECTION("Subtraction") {
        Vec2 c = b - a;
        REQUIRE_THAT(c.x, WithinRel(2.0f, 0.001f));
        REQUIRE_THAT(c.y, WithinRel(2.0f, 0.001f));
    }

    SECTION("Dot Product") {
        float dot = a.dot(b); // 1*3 + 2*4 = 11
        REQUIRE_THAT(dot, WithinRel(11.0f, 0.001f));
    }

    SECTION("Length and Normalize") {
        Vec2 c(3.0f, 4.0f);
        REQUIRE_THAT(c.length(), WithinRel(5.0f, 0.001f));

        Vec2 n = c.normalized();
        REQUIRE_THAT(n.x, WithinRel(3.0f / 5.0f, 0.001f));
        REQUIRE_THAT(n.y, WithinRel(4.0f / 5.0f, 0.001f));
    }
}

TEST_CASE("Vec3 Arithmetic", "[geometry][vec3]") {
    Vec3 a(1.0f, 2.0f, 3.0f);
    Vec3 b(4.0f, 5.0f, 6.0f);

    SECTION("Addition") {
        Vec3 c = a + b;
        REQUIRE_THAT(c.x, WithinRel(5.0f, 0.001f));
        REQUIRE_THAT(c.y, WithinRel(7.0f, 0.001f));
        REQUIRE_THAT(c.z, WithinRel(9.0f, 0.001f));
    }

    SECTION("Dot and Cross Product") {
        REQUIRE_THAT(a.dot(b), WithinRel(32.0f, 0.001f)); // 1*4 + 2*5 + 3*6 = 4 + 10 + 18 = 32
        
        Vec3 cross = a.cross(b); // (2*6 - 3*5, 3*4 - 1*6, 1*5 - 2*4) = (-3, 6, -3)
        // Note: checking exactly 0.0 or negative values with WithinRel can sometimes be tricky 
        // if they cross zero, but for strictly non-zero expected values it's fine.
        // We'll use WithinAbs for safety around 0 just in case.
        REQUIRE_THAT(cross.x, WithinAbs(-3.0f, 0.001f));
        REQUIRE_THAT(cross.y, WithinAbs(6.0f, 0.001f));
        REQUIRE_THAT(cross.z, WithinAbs(-3.0f, 0.001f));
    }
}

TEST_CASE("Vec4 Arithmetic", "[geometry][vec4]") {
    Vec4 a(1.0f, 2.0f, 3.0f, 4.0f);

    SECTION("Scalar Multiplication") {
        Vec4 c = a * 2.0f;
        REQUIRE_THAT(c.x, WithinRel(2.0f, 0.001f));
        REQUIRE_THAT(c.y, WithinRel(4.0f, 0.001f));
        REQUIRE_THAT(c.z, WithinRel(6.0f, 0.001f));
        REQUIRE_THAT(c.w, WithinRel(8.0f, 0.001f));
    }
}

TEST_CASE("Point Arithmetic", "[geometry][point]") {
    Point2 p1(10, 20);
    Point2 p2(5, 5);
    
    Point2 p3 = p1 - p2;
    REQUIRE(p3.x == 5);
    REQUIRE(p3.y == 15);
    
    Point3 p4{1, 2, 3};
    REQUIRE(p4.z == 3);
}

TEST_CASE("Matrix Identity", "[geometry][matrix]") {
    SECTION("Mat3") {
        Mat3 m = Mat3::identity();
        REQUIRE_THAT(m.m[0], WithinRel(1.0f, 0.001f));
        REQUIRE_THAT(m.m[1], WithinAbs(0.0f, 0.001f));
        REQUIRE_THAT(m.m[4], WithinRel(1.0f, 0.001f));
        REQUIRE_THAT(m.m[8], WithinRel(1.0f, 0.001f));
    }

    SECTION("Mat4") {
        Mat4 m = Mat4::identity();
        REQUIRE_THAT(m.m[0], WithinRel(1.0f, 0.001f));
        REQUIRE_THAT(m.m[1], WithinAbs(0.0f, 0.001f));
        REQUIRE_THAT(m.m[5], WithinRel(1.0f, 0.001f));
        REQUIRE_THAT(m.m[10], WithinRel(1.0f, 0.001f));
        REQUIRE_THAT(m.m[15], WithinRel(1.0f, 0.001f));
    }
}

TEST_CASE("Utils", "[geometry][utils]") {
    float rad = degToRad(180.0f);
    // PI ~ 3.14159265
    REQUIRE_THAT(rad, WithinRel(3.1415926535f, 0.001f));
}
