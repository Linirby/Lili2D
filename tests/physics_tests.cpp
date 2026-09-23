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

    SECTION("GetShape") {
        RectShape shape = box1.getShape();
        CHECK(shape.pos == Vec2(0.0f, 0.0f));
        CHECK(shape.size == Vec2(10.0f, 10.0f));

        AABB2 from_shape(shape);
        CHECK(from_shape.min == box1.min);
        CHECK(from_shape.max == box1.max);
    }

    SECTION("RectShape Interoperability") {
        RectShape rect_intersect(Vec2(5.0f, 5.0f), Vec2(10.0f, 10.0f));
        RectShape rect_miss(Vec2(20.0f, 20.0f), Vec2(10.0f, 10.0f));
        RectShape rect_contained(Vec2(2.0f, 2.0f), Vec2(4.0f, 4.0f));

        CHECK(box1.intersect(rect_intersect));
        CHECK_FALSE(box1.intersect(rect_miss));
        CHECK(box1.contains(rect_contained));
        CHECK_FALSE(box1.contains(rect_intersect));
    }

    SECTION("Overlaps (Non-Inclusive)") {
        AABB2 touching_edge(Vec2(10.0f, 0.0f), Vec2(10.0f, 10.0f));
        AABB2 touching_corner(Vec2(10.0f, 10.0f), Vec2(10.0f, 10.0f));

        CHECK(box1.overlaps(box2));
        CHECK(box2.overlaps(box1));
        CHECK_FALSE(box1.overlaps(box3));

        // Touching edge/corner: intersect is true, overlaps is false
        CHECK(box1.intersect(touching_edge));
        CHECK_FALSE(box1.overlaps(touching_edge));

        CHECK(box1.intersect(touching_corner));
        CHECK_FALSE(box1.overlaps(touching_corner));

        RectShape rect_touching(Vec2(10.0f, 0.0f), Vec2(5.0f, 5.0f));
        CHECK(box1.intersect(rect_touching));
        CHECK_FALSE(box1.overlaps(rect_touching));

        RectShape rect_overlapping(Vec2(5.0f, 5.0f), Vec2(5.0f, 5.0f));
        CHECK(box1.overlaps(rect_overlapping));
    }

    SECTION("LineShape Intersection and Overlaps") {
        LineShape line_thru(Vec2(-5.0f, 5.0f), Vec2(15.0f, 5.0f));
        LineShape line_inside(Vec2(2.0f, 2.0f), Vec2(8.0f, 8.0f));
        LineShape line_touching(Vec2(10.0f, -5.0f), Vec2(10.0f, 15.0f));
        LineShape line_corner(Vec2(10.0f, 10.0f), Vec2(15.0f, 15.0f));
        LineShape line_miss(Vec2(12.0f, 0.0f), Vec2(12.0f, 10.0f));
        LineShape point_touching(Vec2(10.0f, 5.0f), Vec2(10.0f, 5.0f));
        LineShape point_inside(Vec2(5.0f, 5.0f), Vec2(5.0f, 5.0f));

        CHECK(box1.intersect(line_thru));
        CHECK(box1.overlaps(line_thru));

        CHECK(box1.intersect(line_inside));
        CHECK(box1.overlaps(line_inside));

        CHECK(box1.intersect(line_touching));
        CHECK_FALSE(box1.overlaps(line_touching));

        CHECK(box1.intersect(line_corner));
        CHECK_FALSE(box1.overlaps(line_corner));

        CHECK_FALSE(box1.intersect(line_miss));
        CHECK_FALSE(box1.overlaps(line_miss));

        CHECK(box1.intersect(point_touching));
        CHECK_FALSE(box1.overlaps(point_touching));

        CHECK(box1.intersect(point_inside));
        CHECK(box1.overlaps(point_inside));
    }

    SECTION("Point Containment, Intersection and Overlaps") {
        CHECK(box1.contains(Vec2(5.0f, 5.0f)));
        CHECK(box1.intersect(Vec2(5.0f, 5.0f)));
        CHECK(box1.overlaps(Vec2(5.0f, 5.0f)));

        CHECK(box1.contains(Vec2(10.0f, 5.0f)));
        CHECK(box1.intersect(Vec2(10.0f, 5.0f)));
        CHECK_FALSE(box1.overlaps(Vec2(10.0f, 5.0f)));

        CHECK_FALSE(box1.contains(Vec2(15.0f, 5.0f)));
        CHECK_FALSE(box1.intersect(Vec2(15.0f, 5.0f)));
        CHECK_FALSE(box1.overlaps(Vec2(15.0f, 5.0f)));
    }
}

TEST_CASE("CircleCollider Intersection and Containment", "[physics][circle]") {
    CircleCollider c1(Vec2(0.0f, 0.0f), 5.0f);
    CircleCollider c2(Vec2(6.0f, 0.0f), 3.0f);
    CircleCollider c3(Vec2(20.0f, 0.0f), 2.0f);
    CircleCollider inner(Vec2(1.0f, 0.0f), 2.0f);

    SECTION("Circle vs Circle Intersection and Overlaps") {
        CHECK(c1.intersect(c2));
        CHECK(c2.intersect(c1));
        CHECK(c1.overlaps(c2));
        CHECK(c2.overlaps(c1));

        CHECK_FALSE(c1.intersect(c3));
        CHECK_FALSE(c3.intersect(c1));
        CHECK_FALSE(c1.overlaps(c3));
        CHECK_FALSE(c3.overlaps(c1));

        // Tangent circles: intersect is true, overlaps is false
        CircleCollider c_tangent(Vec2(8.0f, 0.0f), 3.0f);
        CHECK(c1.intersect(c_tangent));
        CHECK(c_tangent.intersect(c1));
        CHECK_FALSE(c1.overlaps(c_tangent));
        CHECK_FALSE(c_tangent.overlaps(c1));
    }

    SECTION("Point Containment, Intersection and Overlaps") {
        CHECK(c1.contains(Vec2(0.0f, 0.0f)));
        CHECK(c1.intersect(Vec2(0.0f, 0.0f)));
        CHECK(c1.overlaps(Vec2(0.0f, 0.0f)));

        CHECK(c1.contains(Vec2(3.0f, 3.0f)));
        CHECK(c1.intersect(Vec2(3.0f, 3.0f)));
        CHECK(c1.overlaps(Vec2(3.0f, 3.0f)));

        // On boundary
        CHECK(c1.contains(Vec2(5.0f, 0.0f)));
        CHECK(c1.intersect(Vec2(5.0f, 0.0f)));
        CHECK_FALSE(c1.overlaps(Vec2(5.0f, 0.0f)));

        // Outside
        CHECK_FALSE(c1.contains(Vec2(5.1f, 0.0f)));
        CHECK_FALSE(c1.intersect(Vec2(5.1f, 0.0f)));
        CHECK_FALSE(c1.overlaps(Vec2(5.1f, 0.0f)));
    }

    SECTION("Circle vs Circle Containment") {
        CHECK(c1.contains(inner));
        CHECK_FALSE(inner.contains(c1));
        CHECK_FALSE(c1.contains(c2));

        CircleShape inner_shape(Vec2(1.0f, 0.0f), 2.0f);
        CHECK(c1.contains(inner_shape));
    }

    SECTION("Circle vs AABB2 Intersection and Overlaps") {
        AABB2 box(Vec2(3.0f, -2.0f), Vec2(4.0f, 4.0f));
        CHECK(c1.intersect(box));
        CHECK(c1.overlaps(box));
        CHECK(box.intersect(c1));
        CHECK(box.overlaps(c1));

        AABB2 far_box(Vec2(10.0f, 10.0f), Vec2(2.0f, 2.0f));
        CHECK_FALSE(c1.intersect(far_box));
        CHECK_FALSE(c1.overlaps(far_box));
        CHECK_FALSE(far_box.intersect(c1));
        CHECK_FALSE(far_box.overlaps(c1));

        // Tangent circle to box: touches at (5, 0)
        AABB2 tangent_box(Vec2(5.0f, -2.0f), Vec2(4.0f, 4.0f));
        CHECK(c1.intersect(tangent_box));
        CHECK_FALSE(c1.overlaps(tangent_box));
        CHECK(tangent_box.intersect(c1));
        CHECK_FALSE(tangent_box.overlaps(c1));
    }

    SECTION("Circle vs LineShape Intersection and Overlaps") {
        LineShape line_thru(Vec2(-10.0f, 0.0f), Vec2(10.0f, 0.0f));
        LineShape line_tangent(Vec2(-10.0f, 5.0f), Vec2(10.0f, 5.0f));
        LineShape line_miss(Vec2(-10.0f, 6.0f), Vec2(10.0f, 6.0f));
        LineShape point_tangent(Vec2(5.0f, 0.0f), Vec2(5.0f, 0.0f));
        LineShape point_inside(Vec2(2.0f, 0.0f), Vec2(2.0f, 0.0f));

        CHECK(c1.intersect(line_thru));
        CHECK(c1.overlaps(line_thru));

        CHECK(c1.intersect(line_tangent));
        CHECK_FALSE(c1.overlaps(line_tangent));

        CHECK_FALSE(c1.intersect(line_miss));
        CHECK_FALSE(c1.overlaps(line_miss));

        CHECK(c1.intersect(point_tangent));
        CHECK_FALSE(c1.overlaps(point_tangent));

        CHECK(c1.intersect(point_inside));
        CHECK(c1.overlaps(point_inside));
    }

    SECTION("GetShape") {
        CircleShape shape = c1.getShape();
        CHECK(shape.center == Vec2(0.0f, 0.0f));
        CHECK(shape.radius == 5.0f);
        CHECK(shape.segments == 16);

        CircleShape shape32 = c1.getShape(32);
        CHECK(shape32.segments == 32);

        CircleCollider from_shape(shape);
        CHECK(from_shape.center == c1.center);
        CHECK(from_shape.radius == c1.radius);
    }

    SECTION("RectShape Interoperability") {
        RectShape rect_hit(Vec2(3.0f, -2.0f), Vec2(4.0f, 4.0f));
        RectShape rect_miss(Vec2(10.0f, 10.0f), Vec2(2.0f, 2.0f));
        RectShape rect_tangent(Vec2(5.0f, -2.0f), Vec2(4.0f, 4.0f));

        CHECK(c1.intersect(rect_hit));
        CHECK(c1.overlaps(rect_hit));
        CHECK_FALSE(c1.intersect(rect_miss));
        CHECK_FALSE(c1.overlaps(rect_miss));

        CHECK(c1.intersect(rect_tangent));
        CHECK_FALSE(c1.overlaps(rect_tangent));

        CircleCollider from_rect(RectShape(Vec2(0.0f, 0.0f), Vec2(10.0f, 10.0f)));
        CHECK(from_rect.center == Vec2(5.0f, 5.0f));
        CHECK(from_rect.radius == 5.0f);
    }

    SECTION("CircleShape Interoperability") {
        CircleShape cs_overlap(Vec2(6.0f, 0.0f), 3.0f);
        CircleShape cs_tangent(Vec2(8.0f, 0.0f), 3.0f);
        CircleShape cs_miss(Vec2(20.0f, 0.0f), 2.0f);

        CHECK(c1.intersect(cs_overlap));
        CHECK(c1.overlaps(cs_overlap));

        CHECK(c1.intersect(cs_tangent));
        CHECK_FALSE(c1.overlaps(cs_tangent));

        CHECK_FALSE(c1.intersect(cs_miss));
        CHECK_FALSE(c1.overlaps(cs_miss));
    }
}

TEST_CASE("AABB3 Intersection, Overlaps, and Containment", "[physics][aabb3]") {
    AABB3 box1(Vec3(0.0f, 0.0f, 0.0f), Vec3(10.0f, 10.0f, 10.0f));
    AABB3 box2(Vec3(5.0f, 5.0f, 5.0f), Vec3(10.0f, 10.0f, 10.0f));
    AABB3 touching(Vec3(10.0f, 0.0f, 0.0f), Vec3(10.0f, 10.0f, 10.0f));
    AABB3 miss(Vec3(20.0f, 20.0f, 20.0f), Vec3(10.0f, 10.0f, 10.0f));

    SECTION("AABB3 vs AABB3") {
        CHECK(box1.intersect(box2));
        CHECK(box1.overlaps(box2));

        CHECK(box1.intersect(touching));
        CHECK_FALSE(box1.overlaps(touching));

        CHECK_FALSE(box1.intersect(miss));
        CHECK_FALSE(box1.overlaps(miss));
    }

    SECTION("AABB3 Point Tests") {
        Vec3 inside(5.0f, 5.0f, 5.0f);
        Vec3 on_face(10.0f, 5.0f, 5.0f);
        Vec3 outside(15.0f, 5.0f, 5.0f);

        CHECK(box1.contains(inside));
        CHECK(box1.intersect(inside));
        CHECK(box1.overlaps(inside));

        CHECK(box1.contains(on_face));
        CHECK(box1.intersect(on_face));
        CHECK_FALSE(box1.overlaps(on_face));

        CHECK_FALSE(box1.contains(outside));
        CHECK_FALSE(box1.intersect(outside));
        CHECK_FALSE(box1.overlaps(outside));
    }
}
