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
        std::set<std::pair<double, double>> s1 = {{0, 10}, {-5, 0}, {7, 3}, {10, 0}, {-5, 15}};
        EXPECT_EQ(p2.visible({-5, 15}), s1);
        std::set<std::pair<double, double>> s2 = {{7, 7}, {0, 10}, {-5, 0}, {10, 10}};
        EXPECT_EQ(p2.visible({10, 10}), s2);
        std::set<std::pair<double, double>> s3 = {{10, 10}, {0, 10}, {10, 5}, {7, 3}, {-5, 0}, {7, 7}};
        EXPECT_EQ(p2.visible({7, 7}), s3);
        std::set<std::pair<double, double>> s4 = {{-5, 0},{-5, 15}, {0, 10}, {10, 10}, {7, 7}, {10, 5}, {7, 3}, {10, 0}};
        EXPECT_EQ(p2.visible({3, 5}), s4);
    }

    TEST_F (poly_test, test_area_helper) {
        EXPECT_EQ(p1.area_helper({{0, 0}, {0, 1}, {1, 1}, {1, 0}}), 1.0);
        EXPECT_EQ(p2.area_helper({{-5, 0},{-5, 15}, {0, 10}, {10, 10}, {7, 7}, {10, 5}, {7, 3}, {10, 0}}), 147.5);
        poly p4 = poly({{0, 0}, {1, 1}, {2, 0}});
        EXPECT_EQ(p4.area_helper({{0, 0}, {1, 1}, {2, 0}}), 1.0);
        EXPECT_EQ(p4.area_helper({{100.0, 100.0}, {101.0, 101.0}, {102.0, 100.0}}), 1.0);
    }

    TEST_F (poly_test, test_intersect) {
        std::set<std::pair<double, double>> s1 = {{7, 3}, {7, 7}, {7, 10}};
        EXPECT_EQ(p2.intersect({7, 1}, {7, 3}), s1);
        std::set<std::pair<double, double>> s2 = {{7, 3}, {7, 7}, {7, 10}, {7, 0}};
        EXPECT_EQ(p2.intersect({7, 0}, {7, 10}), s2);
        std::set<std::pair<double, double>> s3 = {{10, 0}};
        EXPECT_EQ(p2.intersect({10, 0}, {10, 10}), s3);
        EXPECT_EQ(p2.intersect({10, 0}, {10, 5}), s3);
        std::set<std::pair<double, double>> s4 = {{0, 10}};
        EXPECT_EQ(p2.intersect({3, 5}, {0, 10}), s4);
        std::set<std::pair<double, double>> s5 = {{-5, 0}};
        EXPECT_EQ(p2.intersect({3, 5}, {-5, 0}), s5);
    }

    TEST_F (poly_test, test_sort_vertex) {
        std::set<std::pair<double, double>> s1 = {{0, 10}, {-5, 0}, {7, 7}, {10, 10}};
        std::vector<std::pair<double, double>> v1 = {{-5, 0}, {0, 10}, {10, 10}, {7, 7}};
        EXPECT_EQ(p2.sort_vertex(s1), v1);
        std::set<std::pair<double, double>> s2 = {{-2, 0}, {2, 10}, {10, 5}, {-5, 5}};
        std::vector<std::pair<double, double>> v2 = {{-5, 5}, {2, 10}, {10, 5}, {-2, 0}};
        EXPECT_EQ(p2.sort_vertex(s2), v2);
        std::set<std::pair<double, double>> s3 = {{-2, 0}, {-5, 0}, {10, 0}, {4, 10}, {10, 10}, {8, 8}};
        std::vector<std::pair<double, double>> v3 = {{-5, 0}, {4, 10}, {10, 10}, {8, 8}, {10, 0}, {-2, 0}};
        EXPECT_EQ(p2.sort_vertex(s3), v3);
    }

    TEST_F (poly_test, test_area) {
        EXPECT_EQ(p2.area({0, 6}), 147.5);
        EXPECT_EQ(p2.area({-5, 15}), 112.5);
    }

    TEST_F (poly_test, test_vertex_inside_obstacle) {
        std::vector<std::pair<double, double>> obs = {{-2, 2}, {-2, 5}, {3, 5}, {3, 2}};
        p2.set_obstacle(obs);
        EXPECT_FALSE(p2.inside_obstacle({0, 3}));
        EXPECT_FALSE(p2.inside_obstacle({0, 4.5}));
        EXPECT_FALSE(p2.inside_obstacle({2, 4}));
        EXPECT_TRUE(p2.inside_obstacle({3, 5}));
        EXPECT_TRUE(p2.inside_obstacle({-2, 2}));
        EXPECT_TRUE(p2.inside_obstacle({3, 1}));
        EXPECT_TRUE(p2.inside_obstacle({3, 6}));
        EXPECT_TRUE(p2.inside_obstacle({10, 10}));
        EXPECT_TRUE(p2.inside_obstacle({9, 10}));
        EXPECT_TRUE(p2.inside_obstacle({0, 2}));
        EXPECT_TRUE(p2.inside_obstacle({-5, 15}));
    }

    TEST_F (poly_test, test_segment_inside_obstacle) {
        std::vector<std::pair<double, double>> obs = {{-2, 2}, {-2, 5}, {3, 5}, {3, 2}};
        p2.set_obstacle(obs);
        EXPECT_FALSE(p2.inside_obstacle({0, 3}, {0, 4.5}));
        EXPECT_FALSE(p2.inside_obstacle({7, 3}, {-3, 3}));
        EXPECT_FALSE(p2.inside_obstacle({3, 5}, {2, 4}));
        EXPECT_FALSE(p2.inside_obstacle({0, 3}, {1, 4}));
        EXPECT_FALSE(p2.inside_obstacle({-1, 6}, {5, 0}));
        EXPECT_FALSE(p2.inside_obstacle({2, 2}, {2, 5}));
        EXPECT_TRUE(p2.inside_obstacle({3, 2}, {3, 5}));
        EXPECT_TRUE(p2.inside_obstacle({3, 2}, {-2, 2}));
        EXPECT_TRUE(p2.inside_obstacle({-4, 4}, {0, 0}));
        EXPECT_TRUE(p2.inside_obstacle({-3, 5}, {4, 5}));
        EXPECT_TRUE(p2.inside_obstacle({0, 10}, {7, 3}));
        EXPECT_TRUE(p2.inside_obstacle({-4, 6}, {0, 8}));
    }

    TEST_F (poly_test, test_intersect_obstacle) {
        std::vector<std::pair<double, double>> obs = {{-2, 2}, {-2, 5}, {3, 5}, {3, 2}};
        p2.set_obstacle(obs);
        std::set<std::pair<double, double>> s1 = {{3, 2}, {3, 5}, {3, 10}};
        EXPECT_EQ(p2.intersect_obstacle({3, 1}, {3, 2}), s1);
        std::set<std::pair<double, double>> s2 = {{0, 0}, {-2, 2}, {-5, 5}};
        EXPECT_EQ(p2.intersect_obstacle({0, 0}, {-2, 2}), s2);
        std::set<std::pair<double, double>> s3 = {{10, 0}};
        EXPECT_EQ(p2.intersect_obstacle({10, 0}, {10, 10}), s3);
        EXPECT_EQ(p2.intersect_obstacle({10, 0}, {10, 5}), s3);
        std::set<std::pair<double, double>> s4 = {{0, 10}, {3, 5}};
        EXPECT_EQ(p2.intersect_obstacle({3, 5}, {0, 10}), s4);
    }

    TEST_F (poly_test, test_is_valid_edge) {
        EXPECT_TRUE(p2.is_valid_edge({-5, 0}, {-5, 15}));
        EXPECT_TRUE(p2.is_valid_edge({0, 10}, {10, 10}));
        EXPECT_TRUE(p2.is_valid_edge({10, 10}, {7, 7}));
        EXPECT_TRUE(p2.is_valid_edge({7, 7}, {10, 5}));
        EXPECT_TRUE(p2.is_valid_edge({0, 10}, {1, 10}));
        EXPECT_TRUE(p2.is_valid_edge({0, 10}, {-2, 12}));
        EXPECT_FALSE(p2.is_valid_edge({-5, 0}, {0, 10}));
        EXPECT_FALSE(p2.is_valid_edge({-5, 0}, {10, 10}));
        EXPECT_FALSE(p2.is_valid_edge({-5, 0}, {0, 12}));
        EXPECT_FALSE(p2.is_valid_edge({-2, 2}, {8, 2}));
        EXPECT_FALSE(p2.is_valid_edge({7, 3}, {7, 7}));
    }

}