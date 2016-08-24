#include <gtest/gtest.h>

#include <drawing.h>
#include <linesegment.h>
#include <arc.h>
#include <memory>
#include <parsedxf.h>
#include <dxflib/dl_dxf.h>

class TestDrawing : public ::testing::Test {
    public:
        void SetUp() {
            m_drawing = std::make_shared<Drawing>();
            m_drawing->AddComponent(std::make_shared<LineSegment>(-1,1,1,1));
            m_drawing->AddComponent(std::make_shared<LineSegment>(-1,-1,1,-1));
            m_drawing->AddComponent(std::make_shared<Arc>(1,90,270,pt(-1,0)));
            m_drawing->AddComponent(std::make_shared<Arc>(1,270,90,pt(1,0)));
        }
        void TearDown() {
        }

        std::shared_ptr<Drawing> m_drawing;
};

TEST_F(TestDrawing, TestIntersections) {
    std::list<double> collissions;

    Line testLine(0,0, 0,1);
    m_drawing->GetCollissions(testLine, collissions);

    EXPECT_EQ(2u, collissions.size());

    collissions.sort();
    EXPECT_DOUBLE_EQ(-1.0L, collissions.front());
    collissions.pop_front();
    EXPECT_DOUBLE_EQ(1.0L, collissions.front());

    Line testLine2(1.5,0, 1.5,1);
    collissions.clear();
    m_drawing->GetCollissions(testLine2, collissions);

    ASSERT_EQ(2u, collissions.size());

    collissions.sort();
    EXPECT_DOUBLE_EQ(-sqrt(0.75), collissions.front());
    collissions.pop_front();
    EXPECT_DOUBLE_EQ(sqrt(0.75), collissions.front());

}

TEST_F(TestDrawing, TestAreaFromIntersections) {
    std::list<double> collissions;
    Line testLine2(1.5,0, 1.5,1);
    m_drawing->GetCollissions(testLine2, collissions);
    double area = m_drawing->GetAreaFromIntersections(1.0L, collissions);
    EXPECT_DOUBLE_EQ(sqrt(0.75)*2.0L , area);
}

TEST_F(TestDrawing, TestCalculateArea) {
    double area = m_drawing->CalculateArea(0.01);
    double expected = 4.0L+M_PI;
    EXPECT_TRUE(area<=expected*1.01);
    EXPECT_TRUE(area>=expected*0.99);
}

TEST_F(TestDrawing, TestGetCenter) {
    pt center = m_drawing->GetCenter();
    EXPECT_DOUBLE_EQ(0.0L, center.y);
    EXPECT_TRUE(center.x<=0.01) << "Expected " << 0.0L << " Actual " << center.x;
    EXPECT_TRUE(center.x>=-0.01) << "Expected " << 0.0L << " Actual " << center.x;
}

TEST_F(TestDrawing, TestGetCenterNonOrigin) {
    std::shared_ptr<Drawing> drawing = std::make_shared<Drawing>();
    drawing->AddComponent(std::make_shared<LineSegment>(0,2,2,2));
    drawing->AddComponent(std::make_shared<LineSegment>(0,0,2,0));
    drawing->AddComponent(std::make_shared<Arc>(1,90,270,pt(0,1)));
    drawing->AddComponent(std::make_shared<Arc>(1,270,90,pt(2,1)));

    pt center = drawing->GetCenter();
    EXPECT_DOUBLE_EQ(1.0L, center.y);
    EXPECT_TRUE(center.x<=(1*1.02)) << " expected = " << 1.0L << " actual " << center.x;
    EXPECT_TRUE(center.x>=(1*0.98)) << " expected = " << 1.0L << " actual " << center.x;
}

TEST_F(TestDrawing, TestGetCenterNonOrigin2) {
    //LineSegment {(3.35231, 0.518918) <-> (1.35231, 0.518918)}
    //Arc {r1 @(3.35231, 1.51892), \270 /90s (3.35231, 0.518918), e (3.35231, 2.51892)}
    //LineSegment {(1.35231, 2.51892) <-> (3.35231, 2.51892)}
    //Arc {r1 @(1.35231, 1.51892), \90 /270s (1.35231, 2.51892), e (1.35231, 0.518918)}
    std::shared_ptr<Drawing> drawing = std::make_shared<Drawing>();
    drawing->AddComponent(std::make_shared<LineSegment>(3.35231L,0.518918L,1.35231,0.518918));
    drawing->AddComponent(std::make_shared<Arc>(1.0L,270,90,pt(3.35231,1.51892)));
    drawing->AddComponent(std::make_shared<LineSegment>(1.35231L,2.51892L,3.35231L,2.51892L));
    drawing->AddComponent(std::make_shared<Arc>(1.0L,90,270,pt(1.35231L,1.51892L)));

    drawing->Print(std::cout);
    std::cout << std::endl;

    double yCenter = (2.51892L-0.518918L)/2+0.518918L;
    double xCenter = (3.35231L-1.35231L)/2+1.35231L;
    std::list<double> collissions;
    Line line1(xCenter, 0, xCenter, 10);
    drawing->GetCollissions(line1, collissions);
    collissions.sort();
    ASSERT_EQ(2u, collissions.size());

    pt center = drawing->GetCenter();
    EXPECT_DOUBLE_EQ(yCenter, center.y);
    EXPECT_TRUE(center.x<=(xCenter*1.02));
    EXPECT_TRUE(center.x>=(xCenter*0.98));
}

TEST_F(TestDrawing, TestFromParsedDrawing) {
    ParseDXF parser;
    DL_Dxf dxf;
    dxf.in("test_shape.dxf", &parser);

    Drawing const& drawing(parser.GetDrawing());

    drawing.Print(std::cout);
    std::cout << std::endl;

    double yCenter = (2.51892L-0.518918L)/2+0.518918L;
    double xCenter = (3.35231L-1.35231L)/2+1.35231L;
    std::list<double> collissions;
    Line line1(xCenter, 0, xCenter, 10);
    drawing.GetCollissions(line1, collissions);

    collissions.sort();
    ASSERT_EQ(2u, collissions.size());


    pt center = drawing.GetCenter();

    EXPECT_TRUE(center.y<=(yCenter+0.001)) << "Expected=" << yCenter << " Actual =" << center.y;
    EXPECT_TRUE(center.y>=(yCenter-0.001)) << "Expected=" << yCenter << " Actual =" << center.y;

    EXPECT_TRUE(center.x<=(xCenter+0.001)) << "Expected=" << xCenter << " Actual =" << center.x;
    EXPECT_TRUE(center.x>=(xCenter-0.001)) << "Expected=" << xCenter << " Actual =" << center.x;
}
