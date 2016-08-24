#include <gtest/gtest.h>

#include <line.h>
#include <linesegment.h>

class TestLineSegment : public ::testing::Test {
    public:
        void SetUp() {
        }

        void TearDown() {
        }

};

TEST(TestLineSegment, TestCollissionWithLine) {
    Line line(0,0,10,10); //45 degree line, intersecting origin
    LineSegment seg(0,5, 2,5);

    std::list<pt> noIntersect = seg.Intersection(line);
    EXPECT_EQ(0u, noIntersect.size());

    LineSegment seg2(0,5, 20,6);

    std::list<pt> intersect = seg2.Intersection(line);
    EXPECT_EQ(1u, intersect.size());

    auto int1 = intersect.front();
    EXPECT_DOUBLE_EQ(100.0L/19, int1.x);
    EXPECT_DOUBLE_EQ(100.0L/19, int1.y);
    
}

TEST(TestLineSegment, TestMinMax) {
    LineSegment seg(0,5, 20,5);
    EXPECT_DOUBLE_EQ(20.0, seg.MaxX());
    EXPECT_DOUBLE_EQ(0.0, seg.MinX());
    EXPECT_DOUBLE_EQ(5.0, seg.MaxY());
    EXPECT_DOUBLE_EQ(5.0, seg.MinY());

    LineSegment seg2(-5,5, 20,-50);
    EXPECT_DOUBLE_EQ(20.0, seg2.MaxX());
    EXPECT_DOUBLE_EQ(-5.0, seg2.MinX());
    EXPECT_DOUBLE_EQ(5.0, seg2.MaxY());
    EXPECT_DOUBLE_EQ(-50.0, seg2.MinY());
}

TEST(TestLineSegment, TestHorizontalLineCollission) {
    Line line(0,4, 10,4); //horizontal line at Y=4

    LineSegment seg(1,-50, 1,3);

    std::list<pt> noIntersect = seg.Intersection(line);
    EXPECT_EQ(0u, noIntersect.size());

    LineSegment seg2(0,3, 20,5);

    std::list<pt> intersect = seg2.Intersection(line);
    ASSERT_EQ(1u, intersect.size());

    auto int1 = intersect.front();
    EXPECT_DOUBLE_EQ(10.0L, int1.x);
    EXPECT_DOUBLE_EQ(4.0L, int1.y);

}

TEST(TestLineSegment, TestVerticalLineCollission) {
    Line line(2,0, 2,10); //Vertical line at X=2

    LineSegment seg(1,-50, 1,3);

    std::list<pt> noIntersect = seg.Intersection(line);
    EXPECT_EQ(0u, noIntersect.size());

    LineSegment seg2(1,3, 3,5);

    std::list<pt> intersect = seg2.Intersection(line);
    ASSERT_EQ(1u, intersect.size());

    auto int1 = intersect.front();
    EXPECT_DOUBLE_EQ(2.0L, int1.x);
    EXPECT_DOUBLE_EQ(4.0L, int1.y);
}

TEST(TestLineSegment, TestVerticalLineCollission2) {
    Line line(-5,10, 5,10); //Horzontal line

    LineSegment seg(4,6, 4,11);

    std::list<pt> intersect = seg.Intersection(line);
    ASSERT_EQ(1u, intersect.size());

    auto int1 = intersect.front();
    EXPECT_DOUBLE_EQ(4.0L, int1.x);
    EXPECT_DOUBLE_EQ(10.0L, int1.y);
}

TEST(TestLineSegment, TestRayCollission) {
    Ray ray(pt(0,0), 90);
    LineSegment seg(-4,5, 4,5);

    std::list<pt> intersect = seg.Intersection(ray);

    ASSERT_EQ(1u, intersect.size());

    Ray ray2(pt(0,0), 80);
    intersect = seg.Intersection(ray2);
    ASSERT_EQ(1u, intersect.size());

    Ray ray3(pt(0,0), 270);
    intersect = seg.Intersection(ray3);
    ASSERT_EQ(0u, intersect.size());

    Ray ray4(pt(0,0), 180);
    intersect = seg.Intersection(ray4);
    ASSERT_EQ(0u, intersect.size());
}
