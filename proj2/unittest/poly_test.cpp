#include "poly.h"
#include <gtest/gtest.h>

namespace {
    class poly_test : public testing::Test {
        protected:
            void SetUp() override {
                p1 = poly({{0, 0}, {0, 1}, {1, 1}, {1, 0}});
                p2 = poly({{0, 0}, {0, 10}, {10, 10}, {7, 7}, {10, 5}, {7, 3}, {10, 0}});
            }
        
        poly p1, p2;
    };

    TEST_F (poly_test, test_line_intersect) {
        EXPECT_TRUE(p1.is_line_intersection({0.5, 0.5}, {0.5, 1.5}, {0.5, 0.5}, {1.5, 0.5}));
        EXPECT_TRUE(p1.is_line_intersection({0.5, 0.5}, {1.5, 0.5}, {0.5, 0}, {1.5, 2}));
        EXPECT_TRUE(p1.is_line_intersection({0.5, 0.5}, {1.5, 0.5}, {1.5, 0.5}, {2.5, 0.5}));
        EXPECT_TRUE(p1.is_line_intersection({0.5, 0.5}, {1.5, 0.5}, {1, 0.5}, {2.5, 0.5}));
        EXPECT_TRUE(p1.is_line_intersection({0.5, 0.5}, {1.5, 0.5}, {1, 0.5}, {0, 0}));
        EXPECT_FALSE(p1.is_line_intersection({0.5, 0.5}, {1.5, 0.5}, {2.5, 0.5}, {3.5, 0.5}));
        EXPECT_FALSE(p1.is_line_intersection({0.5, 0.5}, {1.5, 0.5}, {2.5, 0.5}, {2, 0}));
    }

    TEST_F (poly_test, test_inside) {
        EXPECT_TRUE(p2.inside({5, 5}));
        EXPECT_TRUE(p2.inside({10, 10}));
        EXPECT_TRUE(p2.inside({8, 8}));
        EXPECT_FALSE(p2.inside({5, -5}));
        EXPECT_FALSE(p2.inside({8, 7}));
        EXPECT_TRUE(p2.inside({5, 0}));
        EXPECT_TRUE(p2.inside({0, 5}));
    }

}