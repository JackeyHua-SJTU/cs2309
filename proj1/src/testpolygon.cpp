#include "polygon.hpp"
#include <gtest/gtest.h>

namespace {
class polygonTest : public testing::Test {
protected:

    void SetUp() override {
        p1 = Polygon({{0, 0}, {1, 0}, {1, 1}, {0, 1}});
        p2 = Polygon({{0, 0}, {1, 1}, {-1, 1}});
    }
    
    Polygon p1, p2;
};

TEST_F(polygonTest, test_sort_point) {
    p1.sort();
    EXPECT_EQ(p1.points[0].first, 1.0);
    EXPECT_EQ(p1.points[0].second, 0.0);
    EXPECT_EQ(p1.points[1].first, 1.0);
    EXPECT_EQ(p1.points[1].second, 1.0);
    EXPECT_EQ(p1.points[2].first, 0.0);
    EXPECT_EQ(p1.points[2].second, 1.0);
    EXPECT_EQ(p1.points[3].first, 0.0);
    EXPECT_EQ(p1.points[3].second, 0.0);

    p2.sort();
    EXPECT_EQ(p2.points[0].first, 1.0);
    EXPECT_EQ(p2.points[0].second, 1.0);
    EXPECT_EQ(p2.points[1].first, -1.0);
    EXPECT_EQ(p2.points[1].second, 1.0);
    EXPECT_EQ(p2.points[2].first, 0.0);
    EXPECT_EQ(p2.points[2].second, 0.0);
}

TEST_F(polygonTest, test_area) {
    EXPECT_EQ(p1.area(), 1.0);
    EXPECT_EQ(p2.area(), 1.0);

}

} // namespace