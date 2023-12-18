#include "poly.h"
#include <gtest/gtest.h>

namespace {
    class poly_test : public testing::Test {
        protected:
            void SetUp() override {
                p1 = poly({{0, 0}, {0, 1}, {1, 1}, {1, 0}});
                p2 = poly({{-5, 0},{-5, 15}, {0, 10}, {10, 10}, {7, 7}, {10, 5}, {7, 3}, {10, 0}});
                p3 = poly({{0, 0}, {5, 5}, {7, 3}, {10, 3}, {12, 5}, {15, 0}});
            }
        
        poly p1, p2, p3;
    };

    TEST_F (poly_test, test_line_intersect) {
        EXPECT_EQ(p1.is_line_intersection({0.5, 0.5}, {0.5, 1.5}, {0.5, 0.5}, {1.5, 0.5}), -1);
        EXPECT_EQ(p1.is_line_intersection({0.5, 0.5}, {1.5, 0.5}, {0.5, 0}, {1.5, 2}), 1);
        EXPECT_EQ(p1.is_line_intersection({0.5, 0.5}, {1.5, 0.5}, {1.5, 0.5}, {2.5, 0.5}), -2);
        EXPECT_EQ(p1.is_line_intersection({0.5, 0.5}, {1.5, 0.5}, {1, 0.5}, {2.5, 0.5}), -2);
        EXPECT_EQ(p1.is_line_intersection({0.5, 0.5}, {1.5, 0.5}, {1, 0.5}, {0, 0}), -1);
        EXPECT_EQ(p1.is_line_intersection({0.5, 0.5}, {1.5, 0.5}, {2.5, 0.5}, {3.5, 0.5}), 0);
        EXPECT_EQ(p1.is_line_intersection({0.5, 0.5}, {1.5, 0.5}, {2.5, 0.5}, {2, 0}), 0);
        EXPECT_EQ(p1.is_line_intersection({0.5, 0.5}, {1.5, 0.5}, {0.5, 0}, {1.5, 0}), 0);  // parallel
        EXPECT_EQ(p1.is_line_intersection({0.5, 0.5}, {1.5, 0.5}, {2.5, 0.5}, {1, 0.5}), -2); 
    }

    TEST_F (poly_test, test_vertex_inside_p2) {
        EXPECT_TRUE(p2.inside({5, 5}));
        EXPECT_TRUE(p2.inside({10, 10}));
        EXPECT_TRUE(p2.inside({8, 8}));
        EXPECT_FALSE(p2.inside({5, -5}));
        EXPECT_FALSE(p2.inside({8, 7}));
        EXPECT_TRUE(p2.inside({5, 0}));
        EXPECT_TRUE(p2.inside({0, 5}));
        EXPECT_FALSE(p2.inside({-6, 0}));
        EXPECT_TRUE(p2.inside({-3, 10}));
    }

    TEST_F (poly_test, test_vertex_inside_p3) {
        EXPECT_TRUE(p3.inside({5, 3}));
    }

    TEST_F (poly_test, test_get_intersection) {
        EXPECT_EQ(p1.get_intersection({0.5, 0.5}, {1.5, 0.5}, {0.5, 0}, {1.5, 1.0}), std::make_pair(1.0, 0.5));
        //EXPECT_EQ(p1.get_intersection({0.5, 0.5}, {1.5, 0.5}, {1.5, 0.5}, {2.5, 0.5}), std::make_pair(1.5, 0.5));
        EXPECT_EQ(p1.get_intersection({0.0, 0.0}, {0.0, 5.0}, {0.0, 1.5}, {2.0, 1.5}), std::make_pair(0.0, 1.5));
        EXPECT_EQ(p1.get_intersection({0.0, 0.0}, {5.0, 5.0}, {0.0, 5.0}, {5.0, 0.0}), std::make_pair(2.5, 2.5));
        EXPECT_EQ(p1.get_intersection({0.0, 0.0}, {5.0, 5.0}, {1.0, 0.0}, {2.0, 0.0}), std::make_pair((double)INT_MAX, (double)INT_MAX));
        EXPECT_EQ(p1.get_intersection({0.0, 0.0}, {5.0, 0.0}, {1.0, 0.0}, {6.0, 0.0}), std::make_pair((double)INT_MAX, (double)INT_MAX));
        EXPECT_EQ(p1.get_intersection({0.0, 0.0}, {5.0, 0.0}, {1.0, 0.0}, {2.0, 0.0}), std::make_pair((double)INT_MAX, (double)INT_MAX));
    }

    TEST_F (poly_test, test_segment_inside) {
        EXPECT_FALSE(p2.inside({-2, 10}, {-2, 14}));
        EXPECT_TRUE(p2.inside({-2, 10}, {-2, 8}));
        EXPECT_TRUE(p2.inside({-2, 10}, {1, 10}));
        EXPECT_TRUE(p2.inside({-2, 8}, {1, 8}));
        EXPECT_FALSE(p2.inside({0, 14}, {2, 12}));
        EXPECT_FALSE(p2.inside({0, 14}, {5, 14}));
        EXPECT_TRUE(p2.inside({7, 7}, {7, 3}));
        EXPECT_FALSE(p2.inside({10, 10}, {10, 5}));
        EXPECT_FALSE(p2.inside({10, 10}, {10, 0}));
        EXPECT_FALSE(p2.inside({4, 7}, {10, 7}));
    }

    TEST_F (poly_test, test_visible) {
        std::set<std::pair<double, double>> s1 = {{0, 10}, {-5, 0}, {7, 3}, {10, 0}};
        EXPECT_EQ(p2.visible({-5, 15}), s1);
        std::set<std::pair<double, double>> s2 = {{7, 7}, {0, 10}, {-5, 0}};
        EXPECT_EQ(p2.visible({10, 10}), s2);
        std::set<std::pair<double, double>> s3 = {{10, 10}, {0, 10}, {10, 5}, {7, 3}, {-5, 0}};
        EXPECT_EQ(p2.visible({7, 7}), s3);
        std::set<std::pair<double, double>> s4 = {{-5, 15}, {-5, 0}, {0, 10}, {10, 0}, {7, 3}};
    }
}