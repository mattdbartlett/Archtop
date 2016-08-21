#include <gtest/gtest.h>

#include <arc.h>

class TestArc : public ::testing::Test {
    public:
        void SetUp() {
        }

        void TearDown() {
        }

};

typedef Arc<double> DLArc;

TEST(TestArc, TestCollissionWithVeticalLine) {
    DLArc arc(10, 0, 90, pt_base<double>(0,0));

    Line<double> line(5,0, 5,5); //vertical line at 5
    std::list<pt_base<double>> intersections = arc.Intersection(line);

    EXPECT_EQ(1u, intersections.size());

    pt_base<double> int1 = intersections.front();

    EXPECT_DOUBLE_EQ(5.0L, int1.x);
    EXPECT_DOUBLE_EQ(sqrt(100.0L-25.0L), int1.y);
}

TEST(TestArc, TestCollissionWithHorizontalLine) {
    DLArc arc(10, 0, 90, pt_base<double>(1,1));

    Line<double> line(0,5, 5,5); //Horizontal line at y=5
    std::list<pt_base<double>> intersections = arc.Intersection(line);

    EXPECT_EQ(1u, intersections.size());

    pt_base<double> int1 = intersections.front();

    EXPECT_DOUBLE_EQ(1.0L+sqrt(100-16), int1.x);
    EXPECT_DOUBLE_EQ(5.0L, int1.y);
}

TEST(TestArc, TestNoCollission) {
    DLArc arc(10, 0, 350, pt_base<double>(1,1));

    Line<double> line(0,50, 5,55); 
    std::list<pt_base<double>> intersections = arc.Intersection(line);

    EXPECT_EQ(0u, intersections.size());

}

TEST(TestArc, TestCollissionHemicircle) {
    DLArc arc(10, 90, 270, pt_base<double>(0,0));

    Line<double> line(5,0, 5,5); //vertical line at 5
    std::list<pt_base<double>> intersections = arc.Intersection(line);

    ASSERT_EQ(0u, intersections.size());

    Line<double> line2(-5,0, -5,5); //vertical line at x=-5
    std::list<pt_base<double>> intersections2 = arc.Intersection(line2);
    ASSERT_EQ(2u, intersections2.size());

    pt_base<double> int1 = intersections2.front();

    EXPECT_DOUBLE_EQ(-5.0L, int1.x);
    EXPECT_DOUBLE_EQ(sqrt(100.0L-25.0L), int1.y);
}

TEST(TestArc, TestCollissionHemicircle2) {
    DLArc arc(10, 270, 90, pt_base<double>(0,0));

    Line<double> line(5,0, 5,5); //vertical line at 5
    std::list<pt_base<double>> intersections = arc.Intersection(line);

    ASSERT_EQ(2u, intersections.size());
    pt_base<double> int1 = intersections.front();

    EXPECT_DOUBLE_EQ(5.0L, int1.x);
    EXPECT_DOUBLE_EQ(sqrt(100.0L-25.0L), int1.y);


    Line<double> line2(-5,0, -5,5); //vertical line at x=-5
    intersections = arc.Intersection(line2);
    EXPECT_EQ(0u, intersections.size());
}

TEST(TestArc, TestSolveQuadratic) {
    double a=1.0;
    double b=4.0;
    double c=3.0;
    std::list<double> solutions = DLArc().SolveQuadratic(a,b,c);
    solutions.sort();
    EXPECT_DOUBLE_EQ(-3, solutions.front());
    solutions.pop_front();
    EXPECT_DOUBLE_EQ(-1, solutions.front());
}

TEST(TestArc, TestPointsOnArc) {
    DLArc arc(10, 90, 270, pt_base<double>(0,0));

    std::list<pt_base<double>> values;
    values.push_back(pt_base<double>(-10,0)); //yes
    values.push_back(pt_base<double>(0,10)); //yes
    values.push_back(pt_base<double>(0,-10)); //yes
    values.push_back(pt_base<double>(10,-10)); //no
    values.push_back(pt_base<double>(10, 0)); //no
    values.push_back(pt_base<double>(-10/sqrt(2), -10/sqrt(2))); //yes
    std::list<pt_base<double>> results = arc.GetPointsOnArc(values);
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

TEST(TestArc, TestPointsOnArc2) {
    DLArc arc(10, 270, 90, pt_base<double>(0,0));

    std::list<pt_base<double>> values;
    values.push_back(pt_base<double>(-10,0)); //no
    values.push_back(pt_base<double>(0,10)); //yes
    values.push_back(pt_base<double>(0,-10)); //yes
    values.push_back(pt_base<double>(10,-10)); //yes
    values.push_back(pt_base<double>(10, 0)); //yes
    values.push_back(pt_base<double>(-10/sqrt(2), -10/sqrt(2))); //no
    values.push_back(pt_base<double>(10/sqrt(2), -10/sqrt(2))); //yes
    std::list<pt_base<double>> results = arc.GetPointsOnArc(values);
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
   
