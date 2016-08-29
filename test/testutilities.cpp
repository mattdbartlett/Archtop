#include <gtest/gtest.h>

#include <utilities.h>

class TestUtilities : public ::testing::Test {
    public:
        void SetUp() {
        }

        void TearDown() {
        }

};

TEST(TestUtilities, TestCenterFrom2PointsHorizontal) {
    //start with a center of radius 2 centered at 1,1
    auto result = CenterFromTwoPoints<double>(pt(1+sqrt(3),0), pt(1-sqrt(3),0), 2.0L);

    ASSERT_EQ(2u, result.size());
    EXPECT_DOUBLE_EQ(1.0L, result.front().x);
    EXPECT_DOUBLE_EQ(1.0L, result.front().y);

    result.pop_front();
    EXPECT_DOUBLE_EQ(1.0L, result.front().x);
    EXPECT_DOUBLE_EQ(-1.0L, result.front().y);
    

}

TEST(TestUtilities, TestCenterFrom2PointsNotVertical) {
    //start with a center of radius 2 centered at 1,1
    auto result = CenterFromTwoPoints<double>(pt(3,1), pt(1-sqrt(3),0), 2.0L);

    ASSERT_EQ(2u, result.size());
    EXPECT_DOUBLE_EQ(1.0L, result.front().x);
    EXPECT_DOUBLE_EQ(1.0L, result.front().y);

    result.pop_front();
    EXPECT_DOUBLE_EQ(1.0L, result.front().x);
    EXPECT_DOUBLE_EQ(-1.0L, result.front().y);
}
