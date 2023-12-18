#ifndef _POLY_H_
#define _POLY_H_

#include <vector>
#include <set>

class poly {
    private:
        std::vector<std::pair<double, double>> points;
        double min_x, max_x, min_y, max_y; // for wrapper box
        int is_on_line(std::pair<double, double> p, std::pair<double, double> l_src, std::pair<double, double> l_dst);
        double cross(std::vector<double> v1, std::vector<double> v2);
    public:
        poly() = default;
        poly(std::vector<std::pair<double, double>> vc);
        bool inside(std::pair<double, double> p);
        bool inside(std::pair<double, double> src, std::pair<double, double> dst);
        std::set<std::pair<double, double>> visible(std::pair<double, double> src);
        std::set<std::pair<double, double>> intersect(std::pair<double, double> src, std::pair<double, double> dst);
        int is_line_intersection(std::pair<double, double> l1_src, std::pair<double, double> l1_dst, std::pair<double, double> l2_src, std::pair<double, double> l2_dst);
        double area(std::pair<double, double> src);
        //! 需要针对不同的非规范相交情况进行讨论
        //! 如果是平行非规范相交，不考虑，在调用该函数前单独讨论
        std::pair<double, double> get_intersection(std::pair<double, double> l1_src, std::pair<double, double> l1_dst, std::pair<double, double> l2_src, std::pair<double, double> l2_dst); // intersection point (must be regular intersection)
        std::vector<std::pair<double, double>> polygon_intersect(std::vector<std::pair<double, double>> vc1, std::vector<std::pair<double, double>> vc2);
};

#endif