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

typedef Line<double> DLine;

TEST(TestLineSegment, TestCollissionWithLine) {
    DLine line(0,0,10,10); //45 degree line, intersecting origin
    LineSegment<double> seg(0,5, 2,5);

    std::list<pt_base<double>> noIntersect = seg.Intersection(line);
    EXPECT_EQ(0u, noIntersect.size());

    LineSegment<double> seg2(0,5, 20,6);

    std::list<pt_base<double>> intersect = seg2.Intersection(line);
    EXPECT_EQ(1u, intersect.size());

    auto int1 = intersect.front();
    EXPECT_DOUBLE_EQ(100.0L/19, int1.x);
    EXPECT_DOUBLE_EQ(100.0L/19, int1.y);
    
}

TEST(TestLineSegment, TestMinMax) {
    LineSegment<double> seg(0,5, 20,5);
    EXPECT_DOUBLE_EQ(20.0, seg.MaxX());
    EXPECT_DOUBLE_EQ(0.0, seg.MinX());
    EXPECT_DOUBLE_EQ(5.0, seg.MaxY());
    EXPECT_DOUBLE_EQ(5.0, seg.MinY());

    LineSegment<double> seg2(-5,5, 20,-50);
    EXPECT_DOUBLE_EQ(20.0, seg2.MaxX());
    EXPECT_DOUBLE_EQ(-5.0, seg2.MinX());
    EXPECT_DOUBLE_EQ(5.0, seg2.MaxY());
    EXPECT_DOUBLE_EQ(-50.0, seg2.MinY());
}

TEST(TestLineSegment, TestHorizontalLineCollission) {
    DLine line(0,4, 10,4); //horizontal line at Y=4

    LineSegment<double> seg(1,-50, 1,3);

    std::list<pt_base<double>> noIntersect = seg.Intersection(line);
    EXPECT_EQ(0u, noIntersect.size());

    LineSegment<double> seg2(0,3, 20,5);

    std::list<pt_base<double>> intersect = seg2.Intersection(line);
    ASSERT_EQ(1u, intersect.size());

    auto int1 = intersect.front();
    EXPECT_DOUBLE_EQ(10.0L, int1.x);
    EXPECT_DOUBLE_EQ(4.0L, int1.y);

}

TEST(TestLineSegment, TestVerticalLineCollission) {
    DLine line(2,0, 2,10); //Vertical line at X=2

    LineSegment<double> seg(1,-50, 1,3);

    std::list<pt_base<double>> noIntersect = seg.Intersection(line);
    EXPECT_EQ(0u, noIntersect.size());

    LineSegment<double> seg2(1,3, 3,5);

    std::list<pt_base<double>> intersect = seg2.Intersection(line);
    ASSERT_EQ(1u, intersect.size());

    auto int1 = intersect.front();
    EXPECT_DOUBLE_EQ(2.0L, int1.x);
    EXPECT_DOUBLE_EQ(4.0L, int1.y);
}

TEST(TestLineSegment, TestVerticalLineCollission2) {
    DLine line(-5,10, 5,10); //Horzontal line

    LineSegment<double> seg(4,6, 4,11);

    std::list<pt_base<double>> intersect = seg.Intersection(line);
    ASSERT_EQ(1u, intersect.size());

    auto int1 = intersect.front();
    EXPECT_DOUBLE_EQ(4.0L, int1.x);
    EXPECT_DOUBLE_EQ(10.0L, int1.y);
}
