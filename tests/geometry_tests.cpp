#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <cmath>

#include "lili2d/geometry.hpp"
#include "lili2d/world/camera.hpp"

using namespace lili;
using Catch::Matchers::WithinAbs;

TEST_CASE("Vec2 Basic Operations", "[geometry][vec2]") {
    SECTION("Constructors and Default Values") {
        Vec2 v0;
        CHECK(v0.x == 0.0f);
        CHECK(v0.y == 0.0f);

        Vec2 v1(3.0f, 4.0f);
        CHECK(v1.x == 3.0f);
        CHECK(v1.y == 4.0f);
    }

    SECTION("Negation and Arithmetic Operators") {
        Vec2 a(2.0f, -3.0f);
        Vec2 neg = -a;
        CHECK(neg.x == -2.0f);
        CHECK(neg.y == 3.0f);

        Vec2 b(1.0f, 5.0f);
        Vec2 sum = a + b;
        CHECK(sum.x == 3.0f);
        CHECK(sum.y == 2.0f);

        Vec2 diff = a - b;
        CHECK(diff.x == 1.0f);
        CHECK(diff.y == -8.0f);

        Vec2 scaled = a * 2.0f;
        CHECK(scaled.x == 4.0f);
        CHECK(scaled.y == -6.0f);
    }

    SECTION("Compound Assignment Operators") {
        Vec2 v(1.0f, 2.0f);
        v += Vec2(3.0f, 4.0f);
        CHECK(v == Vec2(4.0f, 6.0f));

        v -= Vec2(1.0f, 2.0f);
        CHECK(v == Vec2(3.0f, 4.0f));

        v *= 2.0f;
        CHECK(v == Vec2(6.0f, 8.0f));
    }

    SECTION("Dot and Cross Product") {
        Vec2 a(1.0f, 2.0f);
        Vec2 b(3.0f, 4.0f);
        CHECK(a.dot(b) == 11.0f);
        CHECK(a.cross(b) == -2.0f);
    }

    SECTION("Length and Normalization") {
        Vec2 v(3.0f, 4.0f);
        CHECK_THAT(v.length(), WithinAbs(5.0f, 0.0001f));

        Vec2 norm = v.normalized();
        CHECK_THAT(norm.x, WithinAbs(0.6f, 0.0001f));
        CHECK_THAT(norm.y, WithinAbs(0.8f, 0.0001f));
        CHECK_THAT(norm.length(), WithinAbs(1.0f, 0.0001f));
    }

    SECTION("Lerp") {
        Vec2 a(0.0f, 0.0f);
        Vec2 b(10.0f, 20.0f);
        Vec2 mid = a.lerp(b, 0.5f);
        CHECK(mid.x == 5.0f);
        CHECK(mid.y == 10.0f);
    }
}

TEST_CASE("Vec3 Basic Operations", "[geometry][vec3]") {
    Vec3 a(1.0f, 2.0f, 3.0f);
    Vec3 b(4.0f, 5.0f, 6.0f);

    SECTION("Arithmetic") {
        Vec3 sum = a + b;
        CHECK(sum.x == 5.0f);
        CHECK(sum.y == 7.0f);
        CHECK(sum.z == 9.0f);

        Vec3 diff = b - a;
        CHECK(diff.x == 3.0f);
        CHECK(diff.y == 3.0f);
        CHECK(diff.z == 3.0f);

        Vec3 scaled = a * 2.0f;
        CHECK(scaled.x == 2.0f);
        CHECK(scaled.y == 4.0f);
        CHECK(scaled.z == 6.0f);
    }

    SECTION("Dot Product and Cross Product") {
        CHECK(a.dot(b) == 32.0f);

        Vec3 x(1.0f, 0.0f, 0.0f);
        Vec3 y(0.0f, 1.0f, 0.0f);
        Vec3 z = x.cross(y);
        CHECK(z.x == 0.0f);
        CHECK(z.y == 0.0f);
        CHECK(z.z == 1.0f);
    }
}

TEST_CASE("Vec4 Basic Operations", "[geometry][vec4]") {
    Vec4 a(1.0f, 2.0f, 3.0f, 4.0f);
    Vec4 b(5.0f, 6.0f, 7.0f, 8.0f);

    Vec4 sum = a + b;
    CHECK(sum.x == 6.0f);
    CHECK(sum.y == 8.0f);
    CHECK(sum.z == 10.0f);
    CHECK(sum.w == 12.0f);
}

TEST_CASE("Mat3 Transformations", "[geometry][mat3]") {
    SECTION("Identity Matrix") {
        Mat3 id = Mat3::identity();
        Vec2 p(5.0f, 10.0f);
        Vec2 tp = id.transformPoint(p);
        CHECK(tp.x == 5.0f);
        CHECK(tp.y == 10.0f);
        CHECK(id.determinant() == 1.0f);
    }

    SECTION("Translation") {
        Mat3 t = Mat3::translate(Vec2(10.0f, -5.0f));
        Vec2 p(3.0f, 4.0f);
        Vec2 tp = t.transformPoint(p);
        CHECK(tp.x == 13.0f);
        CHECK(tp.y == -1.0f);

        Vec2 v(3.0f, 4.0f);
        Vec2 tv = t.transformVector(v);
        CHECK(tv.x == 3.0f);
        CHECK(tv.y == 4.0f);
    }

    SECTION("Scale") {
        Mat3 s = Mat3::scale(Vec2(2.0f, 3.0f));
        Vec2 p(4.0f, 5.0f);
        Vec2 tp = s.transformPoint(p);
        CHECK(tp.x == 8.0f);
        CHECK(tp.y == 15.0f);
    }

    SECTION("Rotation") {
        float rad = static_cast<float>(M_PI) / 2.0f;  // 90 degrees
        Mat3 r = Mat3::rotation(rad);
        Vec2 p(1.0f, 0.0f);
        Vec2 tp = r.transformPoint(p);
        CHECK_THAT(tp.x, WithinAbs(0.0f, 0.0001f));
        CHECK_THAT(tp.y, WithinAbs(1.0f, 0.0001f));
    }

    SECTION("Matrix Inversion") {
        Mat3 t = Mat3::translate(Vec2(10.0f, 20.0f));
        Mat3 inv = t.inverse();
        Mat3 result = t * inv;
        Vec2 p(7.0f, 9.0f);
        Vec2 tp = result.transformPoint(p);
        CHECK_THAT(tp.x, WithinAbs(7.0f, 0.0001f));
        CHECK_THAT(tp.y, WithinAbs(9.0f, 0.0001f));
    }

    SECTION("Orthographic Projection") {
        Mat3 ortho = Mat3::orthographic(0.0f, 800.0f, 0.0f, 600.0f);
        Vec2 tl = ortho.transformPoint(Vec2(0.0f, 0.0f));
        CHECK_THAT(tl.x, WithinAbs(-1.0f, 0.0001f));
        CHECK_THAT(tl.y, WithinAbs(1.0f, 0.0001f));

        Vec2 br = ortho.transformPoint(Vec2(800.0f, 600.0f));
        CHECK_THAT(br.x, WithinAbs(1.0f, 0.0001f));
        CHECK_THAT(br.y, WithinAbs(-1.0f, 0.0001f));

        Vec2 center = ortho.transformPoint(Vec2(400.0f, 300.0f));
        CHECK_THAT(center.x, WithinAbs(0.0f, 0.0001f));
        CHECK_THAT(center.y, WithinAbs(0.0f, 0.0001f));
    }

    SECTION("Camera View and Projection") {
        Camera cam;
        cam.setPosition(Vec2(100.0f, 200.0f));
        Mat3 proj = cam.getProjection(800.0f, 600.0f);
        Vec2 tl = proj.transformPoint(Vec2(0.0f, 0.0f));
        CHECK_THAT(tl.x, WithinAbs(-1.0f, 0.0001f));
        CHECK_THAT(tl.y, WithinAbs(1.0f, 0.0001f));

        Mat3 view = cam.getViewMatrix(800.0f, 600.0f);
        Vec2 cam_center = view.transformPoint(Vec2(100.0f, 200.0f));
        CHECK_THAT(cam_center.x, WithinAbs(400.0f, 0.0001f));
        CHECK_THAT(cam_center.y, WithinAbs(300.0f, 0.0001f));

        Vec2 ndc_center = (proj * view).transformPoint(Vec2(100.0f, 200.0f));
        CHECK_THAT(ndc_center.x, WithinAbs(0.0f, 0.0001f));
        CHECK_THAT(ndc_center.y, WithinAbs(0.0f, 0.0001f));

        // In 2D, y increases downwards. Point below camera (y = 250) should map below center (NDC y < 0)
        Vec2 below = (proj * view).transformPoint(Vec2(100.0f, 250.0f));
        CHECK_THAT(below.x, WithinAbs(0.0f, 0.0001f));
        CHECK(below.y < 0.0f);
    }
}

TEST_CASE("Math Utils", "[geometry][utils]") {
    CHECK_THAT(degToRad(180.0f), WithinAbs(static_cast<float>(M_PI), 0.0001f));
    CHECK_THAT(radToDeg(static_cast<float>(M_PI)), WithinAbs(180.0f, 0.0001f));
}

TEST_CASE(
    "Geometry Type Traits and Compile-Time Constants", "[geometry][traits]"
) {
    // Verify sizeof <= 16 bytes for pass-by-value guarantees
    static_assert(sizeof(Vec2) == 8);
    static_assert(sizeof(Point2) == 8);
    static_assert(sizeof(Vec3) == 12);
    static_assert(sizeof(Point3) == 12);
    static_assert(sizeof(Vec4) == 16);
    static_assert(sizeof(CircleShape) == 16);
    static_assert(sizeof(RectShape) == 16);

    // Verify trivial / nothrow move constructors
    static_assert(std::is_nothrow_move_constructible_v<Vec2>);
    static_assert(std::is_nothrow_move_constructible_v<Vec3>);
    static_assert(std::is_nothrow_move_constructible_v<Vec4>);
    static_assert(std::is_nothrow_move_constructible_v<Point2>);
    static_assert(std::is_nothrow_move_constructible_v<Point3>);
    static_assert(std::is_nothrow_move_constructible_v<Mat3>);
    static_assert(std::is_nothrow_move_constructible_v<Mat4>);

    // Verify constexpr operations
    constexpr Vec2 cv(3.0f, 4.0f);
    constexpr Vec2 cv2 = cv + Vec2(1.0f, 2.0f);
    static_assert(cv2.x == 4.0f && cv2.y == 6.0f);

    constexpr float dot_val = cv.dot(cv2);
    static_assert(dot_val == 36.0f);

    constexpr RectShape rect_a(Vec2(10.0f, 20.0f), Vec2(30.0f, 40.0f));
    static_assert(rect_a.pos.x == 10.0f && rect_a.pos.y == 20.0f);
    static_assert(rect_a.size.x == 30.0f && rect_a.size.y == 40.0f);
    static_assert(rect_a.contains(Vec2(15.0f, 25.0f)));
    static_assert(!rect_a.contains(Vec2(5.0f, 25.0f)));

    constexpr RectShape rect_b(10.0f, 20.0f, 30.0f, 40.0f);
    static_assert(rect_b.pos.x == 10.0f && rect_b.size.y == 40.0f);

    CHECK(true);
}
