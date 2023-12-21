#ifndef _POLY_H_
#define _POLY_H_

#include <vector>
#include <set>

class poly {
    private:
        std::vector<std::pair<double, double>> points;
        double min_x, max_x, min_y, max_y; // for wrapper box
        bool flag;  // true if there is an obstacle
        std::vector<std::pair<double, double>> vertex_inside;
        std::vector<std::pair<double, double>> obstacle;

        int is_on_line(std::pair<double, double> p, std::pair<double, double> l_src, std::pair<double, double> l_dst);
        double cross(std::vector<double> v1, std::vector<double> v2);

    public:
        // public variables
        double area_covered;
        std::pair<double, double> pos;  //* for one-camera case
        std::vector<std::pair<double, double>> pos_in_pair; //* only for two-camera case
        std::vector<std::pair<double, double>> vertex_set;
        
        // initialization functions
        poly();
        poly(std::vector<std::pair<double, double>> vc, bool flag = false);
        inline void set_obstacle(std::vector<std::pair<double, double>> vc) {obstacle = vc;}
        void add_point(std::pair<double, double> p); //* for console input
        void add_curve(std::pair<double, double> src, std::pair<double, double> dst, std::pair<double, double> ctrl);  //* for console curve input
        void set_inside_set();  //* for two-camera case

        // helper functions
        bool is_vertex(std::pair<double, double> p);
        bool is_valid_edge(std::pair<double, double> src, std::pair<double, double> dst);
        bool vertex_on_edge(std::pair<double, double> p);
        bool is_valid_obstacle();

        // get private variables
        inline std::vector<std::pair<double, double>> get_points() {return points;}
        inline bool get_flag() {return flag;}
        inline auto get_obstacle() {return obstacle;}

        // core judging functions
        bool inside(std::pair<double, double> p);
        bool inside(std::pair<double, double> src, std::pair<double, double> dst);
        bool inside_obstacle(std::pair<double, double> p);
        bool inside_obstacle(std::pair<double, double> src, std::pair<double, double> dst);
        std::set<std::pair<double, double>> intersect(std::pair<double, double> src, std::pair<double, double> dst);
        std::set<std::pair<double, double>> intersect_obstacle(std::pair<double, double> src, std::pair<double, double> dst);
        double area_helper(std::vector<std::pair<double, double>> vc);
        int is_line_intersection(std::pair<double, double> l1_src, std::pair<double, double> l1_dst, std::pair<double, double> l2_src, std::pair<double, double> l2_dst);
        //! 如果是平行非规范相交，不考虑，在调用该函数前单独讨论
        std::pair<double, double> get_intersection(std::pair<double, double> l1_src, std::pair<double, double> l1_dst, std::pair<double, double> l2_src, std::pair<double, double> l2_dst); 

        // high level interface
        void execute_one_camera();
        void execute_two_camera();
        double area(std::pair<double, double> src);
        std::set<std::pair<double, double>> visible(std::pair<double, double> src);
        std::vector<std::pair<double, double>> get_vertex_set(std::pair<double, double> src);
        std::vector<std::pair<double, double>> sort_vertex(std::set<std::pair<double, double>> st);
        std::vector<std::pair<double, double>> sort_vertex_obstacle(std::set<std::pair<double, double>> st,std::pair<double, double> src);
        std::vector<std::pair<double, double>> polygon_intersect(std::vector<std::pair<double, double>> vc1, std::vector<std::pair<double, double>> vc2);
};

#endif