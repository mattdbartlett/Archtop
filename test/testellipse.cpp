#include <gtest/gtest.h>

#include <ellipse.h>

class TestEllipse : public ::testing::Test {
    public:
        void SetUp() {
        }

        void TearDown() {
        }

};

TEST(TestEllipse, TestCollissionWithVerticalLine) {
    Ellipse ellipse(10, 10, pt(0,0));

    Line line(5,0, 5,5); //vertical line at 5
    std::list<pt> intersections = ellipse.Intersection(line);

    ASSERT_EQ(2u, intersections.size());

    pt int1 = intersections.front();

    EXPECT_DOUBLE_EQ(5.0L, int1.x);
    EXPECT_DOUBLE_EQ(sqrt(100.0L-25.0L), int1.y);

    intersections.pop_front();
    int1 = intersections.front();
    EXPECT_DOUBLE_EQ(5.0L, int1.x);
    EXPECT_DOUBLE_EQ(-sqrt(100.0L-25.0L), int1.y);
}

TEST(TestEllipse, TestCollissionWithHorizontalLine) {
    Ellipse ellipse(10, 10, pt(1,1));

    Line line(0,5, 5,5); //Horizontal line at y=5
    std::list<pt> intersections = ellipse.Intersection(line);

    ASSERT_EQ(2u, intersections.size());

    pt int1 = intersections.front();

    EXPECT_DOUBLE_EQ(1.0L+sqrt(100-16), int1.x);
    EXPECT_DOUBLE_EQ(5.0L, int1.y);

    intersections.pop_front();
    int1 = intersections.front();
    EXPECT_DOUBLE_EQ(1.0L-sqrt(100-16), int1.x);
    EXPECT_DOUBLE_EQ(5.0L, int1.y);
}
/*
TEST(TestEllipse, TestNoCollission) {
    Arc arc(10, 0, 350, pt(1,1));

    Line line(0,50, 5,55); 
    std::list<pt> intersections = arc.Intersection(line);

    EXPECT_EQ(0u, intersections.size());

}

TEST(TestEllipse, TestCollissionHemicircle) {
    Arc arc(10, 90, 270, pt(0,0));

    Line line(5,0, 5,5); //vertical line at 5
    std::list<pt> intersections = arc.Intersection(line);

    ASSERT_EQ(0u, intersections.size());

    Line line2(-5,0, -5,5); //vertical line at x=-5
    std::list<pt> intersections2 = arc.Intersection(line2);
    ASSERT_EQ(2u, intersections2.size());

    pt int1 = intersections2.front();

    EXPECT_DOUBLE_EQ(-5.0L, int1.x);
    EXPECT_DOUBLE_EQ(sqrt(100.0L-25.0L), int1.y);
}

TEST(TestEllipse, TestCollissionHemicircle2) {
    Arc arc(10, 270, 90, pt(0,0));

    Line line(5,0, 5,5); //vertical line at 5
    std::list<pt> intersections = arc.Intersection(line);

    ASSERT_EQ(2u, intersections.size());
    pt int1 = intersections.front();

    EXPECT_DOUBLE_EQ(5.0L, int1.x);
    EXPECT_DOUBLE_EQ(sqrt(100.0L-25.0L), int1.y);


    Line line2(-5,0, -5,5); //vertical line at x=-5
    intersections = arc.Intersection(line2);
    EXPECT_EQ(0u, intersections.size());
}

TEST(TestEllipse, TestCollissionHemicircle2WithRay) {
    Arc arc(10, 270, 90, pt(0,0));

    Ray ray1(pt(5,0), 90.0L); //vertical line at 5
    std::list<pt> intersections = arc.Intersection(ray1);

    ASSERT_EQ(1u, intersections.size());
    pt int1 = intersections.front();

    EXPECT_DOUBLE_EQ(5.0L, int1.x);
    EXPECT_DOUBLE_EQ(sqrt(100.0L-25.0L), int1.y);


    Ray ray2(pt(5,0), 270.0L); //vertical line at 5, pointing down

    intersections = arc.Intersection(ray2);
    EXPECT_EQ(1u, intersections.size());

    int1 = intersections.front();

    EXPECT_DOUBLE_EQ(5.0L, int1.x);
    EXPECT_DOUBLE_EQ(-sqrt(100.0L-25.0L), int1.y);
}

TEST(TestEllipse, TestCollissionQuarterCircle) {
    Arc arc(10, 90, 180, pt(0,0));

    Line line(0,0, -5,5); //diagonal line at origin
    std::list<pt> intersections = arc.Intersection(line);

    ASSERT_EQ(1u, intersections.size());
    pt int1 = intersections.front();

    EXPECT_DOUBLE_EQ(-10.0L/sqrt(2), int1.x);
    EXPECT_DOUBLE_EQ(10.0L/sqrt(2), int1.y);


    Line line2(0,0, 5,5); //diagonal line at origin
    intersections = arc.Intersection(line2);
    EXPECT_EQ(0u, intersections.size());

}

TEST(TestEllipse, TestCollissionQuarterCircleWithRay) {
    Arc arc(10, 90, 180, pt(0,0));

    Ray ray1(pt(-5,0), 90.0L); //vertical line at -5
    std::list<pt> intersections = arc.Intersection(ray1);

    ASSERT_EQ(1u, intersections.size());
    pt int1 = intersections.front();

    EXPECT_DOUBLE_EQ(-5.0L, int1.x);
    EXPECT_DOUBLE_EQ(sqrt(100.0L-25.0L), int1.y);

    Ray ray2(pt(-5,0), 270.0L); //vertical line at 5, pointing down

    intersections = arc.Intersection(ray2);
    ASSERT_EQ(0u, intersections.size());

    Ray ray3(pt(0,0), 135.0L); //diagonal line at center

    intersections = arc.Intersection(ray3);
    ASSERT_EQ(1u, intersections.size());

    int1 = intersections.front();

    EXPECT_DOUBLE_EQ(-10.0L/sqrt(2), int1.x);
    EXPECT_DOUBLE_EQ(10.0L/sqrt(2), int1.y);

}

TEST(TestEllipse, TestSolveQuadratic) {
    double a=1.0;
    double b=4.0;
    double c=3.0;
    std::list<double> solutions = SolveQuadratic(a,b,c);
    solutions.sort();
    EXPECT_DOUBLE_EQ(-3, solutions.front());
    solutions.pop_front();
    EXPECT_DOUBLE_EQ(-1, solutions.front());
}

TEST(TestEllipse, TestPointsOnArc) {
    Arc arc(10, 90, 270, pt(0,0));

    std::list<pt> values;
    values.push_back(pt(-10,0)); //yes
    values.push_back(pt(0,10)); //yes
    values.push_back(pt(0,-10)); //yes
    values.push_back(pt(10,-10)); //no
    values.push_back(pt(10, 0)); //no
    values.push_back(pt(-10/sqrt(2), -10/sqrt(2))); //yes
    std::list<pt> results = arc.GetPointsOnArc(values);
    EXPECT_EQ(4u, results.size());

    EXPECT_DOUBLE_EQ(-10.0L, results.front().x);
    EXPECT_DOUBLE_EQ(-0.0L, results.front().y);
    results.pop_front();

    EXPECT_DOUBLE_EQ(0.0L, results.front().x);
    EXPECT_DOUBLE_EQ(10.0L, results.front().y);
    results.pop_front();

    EXPECT_DOUBLE_EQ(0.0L, results.front().x);
    EXPECT_DOUBLE_EQ(-10.0L, results.front().y);
    results.pop_front();

    EXPECT_DOUBLE_EQ(-10.0L/sqrt(2), results.front().x);
    EXPECT_DOUBLE_EQ(-10.0L/sqrt(2), results.front().y);
    results.pop_front();
}

TEST(TestEllipse, TestPointsOnArc2) {
    Arc arc(10, 270, 90, pt(0,0));

    std::list<pt> values;
    values.push_back(pt(-10,0)); //no
    values.push_back(pt(0,10)); //yes
    values.push_back(pt(0,-10)); //yes
    values.push_back(pt(10,-10)); //yes
    values.push_back(pt(10, 0)); //yes
    values.push_back(pt(-10/sqrt(2), -10/sqrt(2))); //no
    values.push_back(pt(10/sqrt(2), -10/sqrt(2))); //yes
    std::list<pt> results = arc.GetPointsOnArc(values);
    ASSERT_EQ(5u, results.size());

    EXPECT_DOUBLE_EQ(0.0L, results.front().x);
    EXPECT_DOUBLE_EQ(10.0L, results.front().y);
    results.pop_front();

    EXPECT_DOUBLE_EQ(0.0L, results.front().x);
    EXPECT_DOUBLE_EQ(-10.0L, results.front().y);
    results.pop_front();

    EXPECT_DOUBLE_EQ(10.0L, results.front().x);
    EXPECT_DOUBLE_EQ(-10.0L, results.front().y);
    results.pop_front();

    EXPECT_DOUBLE_EQ(10.0L, results.front().x);
    EXPECT_DOUBLE_EQ(0.0L, results.front().y);
    results.pop_front();

    EXPECT_DOUBLE_EQ(10.0L/sqrt(2), results.front().x);
    EXPECT_DOUBLE_EQ(-10.0L/sqrt(2), results.front().y);
    results.pop_front();
}
  */ 
