#ifndef _POLY_H_
#define _POLY_H_

#include <vector>
#include <set>

class poly {
    private:
        std::vector<std::pair<double, double>> points;
        double min_x, max_x, min_y, max_y; // for wrapper box
        int is_on_line(std::pair<double, double> p, std::pair<double, double> l_src, std::pair<double, double> l_dst);
    public:
        poly() = default;
        poly(std::vector<std::pair<double, double>> vc);
        bool inside(std::pair<double, double> p);
        std::vector<std::pair<double, double>> visible(std::pair<double, double> src);
        std::set<std::pair<double, double>> intersect(std::pair<double, double> src, std::pair<double, double> dst);
        int is_line_intersection(std::pair<double, double> l1_src, std::pair<double, double> l1_dst, std::pair<double, double> l2_src, std::pair<double, double> l2_dst);
};

#endif