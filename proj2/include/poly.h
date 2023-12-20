#ifndef _POLY_H_
#define _POLY_H_

#include <vector>
#include <set>

class poly {
    private:
        std::vector<std::pair<double, double>> points;
        double min_x, max_x, min_y, max_y; // for wrapper box
        bool flag;  // if choose two cameras, then flag is true
        std::vector<std::pair<double, double>> vertex_inside;
        int is_on_line(std::pair<double, double> p, std::pair<double, double> l_src, std::pair<double, double> l_dst);
        double cross(std::vector<double> v1, std::vector<double> v2);

    public:
        inline std::vector<std::pair<double, double>> get_points() {return points;}
        double area_covered;
        std::pair<double, double> pos;  // for one-camera case
        std::vector<std::pair<double, double>> pos_in_pair; // only for two-camera case
        std::vector<std::pair<double, double>> vertex_set;
        poly();
        poly(std::vector<std::pair<double, double>> vc, bool flag = false);
        bool inside(std::pair<double, double> p);
        bool inside(std::pair<double, double> src, std::pair<double, double> dst);
        std::set<std::pair<double, double>> visible(std::pair<double, double> src);
        std::set<std::pair<double, double>> intersect(std::pair<double, double> src, std::pair<double, double> dst);
        std::vector<std::pair<double, double>> sort_vertex(std::set<std::pair<double, double>> st);
        int is_line_intersection(std::pair<double, double> l1_src, std::pair<double, double> l1_dst, std::pair<double, double> l2_src, std::pair<double, double> l2_dst);
        double area(std::pair<double, double> src);
        //! 需要针对不同的非规范相交情况进行讨论
        //! 如果是平行非规范相交，不考虑，在调用该函数前单独讨论
        std::pair<double, double> get_intersection(std::pair<double, double> l1_src, std::pair<double, double> l1_dst, std::pair<double, double> l2_src, std::pair<double, double> l2_dst); // intersection point (must be regular intersection)
        std::vector<std::pair<double, double>> polygon_intersect(std::vector<std::pair<double, double>> vc1, std::vector<std::pair<double, double>> vc2);
        double area_helper(std::vector<std::pair<double, double>> vc);
        std::vector<std::pair<double, double>> get_vertex_set(std::pair<double, double> src);
        void execute_one_camera();
        void execute_two_camera();
        void add_point(std::pair<double, double> p);
        void add_curve(std::pair<double, double> src, std::pair<double, double> dst, std::pair<double, double> ctrl);
        void set_inside_set();
};

#endif