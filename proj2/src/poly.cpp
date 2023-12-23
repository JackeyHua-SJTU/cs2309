#include "poly.h"
#include <unordered_set>
#include <deque>
#include <algorithm>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/foreach.hpp>
#include <omp.h>
#include <map>
#include <cmath>
#include <iostream>
#include <random>

namespace bg = boost::geometry;

typedef bg::model::d2::point_xy<double> Point;
typedef bg::model::polygon<Point> Polygon;

poly::poly(std::vector<std::pair<double, double>> vc, bool flag) : points(vc), min_x(INT_MAX), max_x(INT_MIN), min_y(INT_MAX), max_y(INT_MIN) {
    for (auto&& [x, y] : this->points) {
        min_x = std::min(min_x, x);
        max_x = std::max(max_x, x);
        min_y = std::min(min_y, y);
        max_y = std::max(max_y, y);
    }
    this->flag = flag;
    this->area_covered = 0.0;
}

poly::poly() {
    min_x = INT_MAX;
    max_x = INT_MIN;
    min_y = INT_MAX;
    max_y = INT_MIN;
    this->area_covered = 0.0;
    this->flag = false;
    points.clear();
}

//* return true if src is on the edge of polygon specified by points
//* used in judging whether a point is inside a polygon with obstacle inside
bool poly::vertex_on_edge(std::pair<double, double> src) {
    for (int i = 0; i < points.size(); ++i) {
        auto cur = points[i];
        auto next = points[(i + 1) % points.size()];
        if (is_on_line(src, cur, next) == 1) {
            return true;
        }
    }
    return false;
}

//* return true if segment (src, dst) is part of an edge, or just an edge
bool poly::is_valid_edge(std::pair<double, double> src, std::pair<double, double> dst) {
    if (src == dst) {
        return false;
    }
    int size = points.size();
    for (int i = 0; i < size; ++i) {
        auto cur = points[i];
        auto next = points[(i + 1) % size];
        if (is_on_line(src, cur, next) == 1 && is_on_line(dst, cur, next) == 1) {
            return true;
        }
    }

    if (flag) {
        for (int i = 0; i < obstacle.size(); ++i) {
            auto cur = obstacle[i];
            auto next = obstacle[(i + 1) % obstacle.size()];
            if (is_on_line(src, cur, next) == 1 && is_on_line(dst, cur, next) == 1) {
                return true;
            }
        }
    }
    return false;
}

//* cross product of two vectors
double poly::cross(std::vector<double> v1, std::vector<double> v2) {
    return v1[0] * v2[1] - v1[1] * v2[0];
}

//* console input of vertex set
void poly::add_point(std::pair<double, double> p) {
    points.emplace_back(p);
    min_x = std::min(min_x, p.first);
    max_x = std::max(max_x, p.first);
    min_y = std::min(min_y, p.second);
    max_y = std::max(max_y, p.second);
}

//* console input of vertex set 
//! special case: input a curve, should assign the control point (center), src and dst
//* gap in the curve is 0.1, subject to changes if needed
void poly::add_curve(std::pair<double, double> src, std::pair<double, double> dst, std::pair<double, double> ctrl) {
    double angle_1 = atan2(src.second - ctrl.second, src.first - ctrl.first);
    double angle_2 = atan2(dst.second - ctrl.second, dst.first - ctrl.first);
    if (angle_1 < 0) {
        angle_1 += 2 * M_PI;
    }
    if (angle_2 < 0) {
        angle_2 += 2 * M_PI;
    }

    double radius = sqrt(pow(src.first - ctrl.first, 2) + pow(src.second - ctrl.second, 2));

    if (angle_1 > angle_2) {
        std::swap(angle_1, angle_2);
    }

    for (double angle = angle_1; angle <= angle_2; angle += 0.1) {
        double x = ctrl.first + radius * cos(angle);
        double y = ctrl.second + radius * sin(angle);
        std::cout << x << " " << y << std::endl;
        add_point({x, y});
    }
}

//* return 1 if regular intersection, -2 if irregular intersection (parallel)
//* return -1 if irregular intersection (colinear), 0 if no intersection
//! intersection of two SEGMENTS
int poly::is_line_intersection(std::pair<double, double> l1_src, std::pair<double, double> l1_dst, std::pair<double, double> l2_src, std::pair<double, double> l2_dst) {
    // regular intersection
    const double eps = 1e-6;
    std::vector<double> l1 = {l1_dst.first - l1_src.first, l1_dst.second - l1_src.second};
    std::vector<double> l2 = {l2_dst.first - l2_src.first, l2_dst.second - l2_src.second};
    double cross_l1_l2_src = l1[0] * (l2_src.second - l1_src.second) - l1[1] * (l2_src.first - l1_src.first);
    double cross_l1_l2_dst = l1[0] * (l2_dst.second - l1_src.second) - l1[1] * (l2_dst.first - l1_src.first);
    double cross_l2_l1_src = l2[0] * (l1_src.second - l2_src.second) - l2[1] * (l1_src.first - l2_src.first);
    double cross_l2_l1_dst = l2[0] * (l1_dst.second - l2_src.second) - l2[1] * (l1_dst.first - l2_src.first);
    if ((cross_l1_l2_src * cross_l1_l2_dst < -eps) && (cross_l2_l1_src * cross_l2_l1_dst < -eps)) {
        return 1;
    }
    // irregular intersection
    // colinear
    if (abs(cross_l1_l2_dst) <= eps && abs(cross_l1_l2_src) <= eps && abs(cross_l2_l1_dst) <= eps && abs(cross_l2_l1_src) <= eps) {
        std::vector<double> l1_src_to_l2_src = {l2_src.first - l1_src.first, l2_src.second - l1_src.second};
        std::vector<double> l1_src_to_l2_dst = {l2_dst.first - l1_src.first, l2_dst.second - l1_src.second};
        std::vector<double> l1_dst_to_l2_src = {l2_src.first - l1_dst.first, l2_src.second - l1_dst.second};
        std::vector<double> l1_dst_to_l2_dst = {l2_dst.first - l1_dst.first, l2_dst.second - l1_dst.second};
        if (l1_src_to_l2_src[0] * l1_src_to_l2_dst[0] + l1_src_to_l2_src[1] * l1_src_to_l2_dst[1] > 0 && 
            l1_dst_to_l2_src[0] * l1_dst_to_l2_dst[0] + l1_dst_to_l2_src[1] * l1_dst_to_l2_dst[1] > 0) {
            return 0;
        } else {
            return -2;
        }
    }

    std::pair<double, double> start, end, cur;
    if (abs(cross_l1_l2_src) <= eps || abs(cross_l1_l2_dst) <= eps) {
        start = l1_src;
        end = l1_dst;
        if (abs(cross_l1_l2_src) <= eps) {
            cur = l2_src;
        } else {
            cur = l2_dst;
        }
    } else if (abs(cross_l2_l1_src) <= eps || abs(cross_l2_l1_dst) <= eps) {
        start = l2_src;
        end = l2_dst;
        if (abs(cross_l2_l1_src) <= eps) {
            cur = l1_src;
        } else {
            cur = l1_dst;
        }
    } else {
        return 0;
    }
    std::vector<double> cur_to_start = {start.first - cur.first, start.second - cur.second};
    std::vector<double> cur_to_end = {end.first - cur.first, end.second - cur.second};
    if (cur_to_start[0] * cur_to_end[0] + cur_to_start[1] * cur_to_end[1] <= 0) {
        return -1;
    }
    return 0;
}

//* return the intersection point of two lines
//* return (INT_MAX, INT_MAX) if no intersection, parrallel intersection
std::pair<double, double> poly::get_intersection(std::pair<double, double> l1_src, std::pair<double, double> l1_dst, std::pair<double, double> l2_src, std::pair<double, double> l2_dst) {
    int intersection = is_line_intersection(l1_src, l1_dst, l2_src, l2_dst);
    if (intersection == 0 || intersection == -2) {
        return std::make_pair(INT_MAX, INT_MAX);
    }
    if (intersection == 1) {
        std::vector<double> l1 = {l1_dst.first - l1_src.first, l1_dst.second - l1_src.second};
        std::vector<double> l1_src_to_l2_src = {l2_src.first - l1_src.first, l2_src.second - l1_src.second};
        std::vector<double> l1_src_to_l2_dst = {l2_dst.first - l1_src.first, l2_dst.second - l1_src.second};
        auto lambda = abs(cross(l1_src_to_l2_src, l1) / cross(l1, l1_src_to_l2_dst));
        return std::make_pair(l2_src.first + lambda * (l2_dst.first - l2_src.first) / (1 + lambda),
                                l2_src.second + lambda * (l2_dst.second - l2_src.second) / (1 + lambda));
    } else if (intersection == -1) {
        if (is_on_line(l2_src, l1_src, l1_dst) == 1) {
            return l2_src;
        } else if (is_on_line(l2_dst, l1_src, l1_dst) == 1) {
            return l2_dst;
        } else if (is_on_line(l1_src, l2_src, l2_dst) == 1) {
            return l1_src;
        } else if (is_on_line(l1_dst, l2_src, l2_dst) == 1) {
            return l1_dst;
        }
    }
    return std::make_pair(INT_MAX, INT_MAX);
}

//* return 1 if p is on the segment, 0 if not, -1 if on the extension of the segment
int poly::is_on_line(std::pair<double, double> p, std::pair<double, double> l_src, std::pair<double, double> l_dst) {
    const double eps = 1e-6;
    std::vector<double> l = {l_dst.first - l_src.first, l_dst.second - l_src.second};
    std::vector<double> p_to_src = {l_src.first - p.first, l_src.second - p.second};
    std::vector<double> p_to_dst = {l_dst.first - p.first, l_dst.second - p.second};
    
    if (abs(l[0] * p_to_src[1] - l[1] * p_to_src[0]) > eps) {
        return 0;
    }

    if (p_to_src[0] * p_to_dst[0] + p_to_src[1] * p_to_dst[1] <= 0) {
        return 1;
    }
    
    return -1;
}

//* return true if p is inside the polygon
bool poly::inside(std::pair<double, double> p) {
    const double eps = 1e-6;
    int cnt = 0;
    std::pair<double, double> end = {max_x + 10, p.second};
    int size = points.size();

    for (int i = 0; i < size; ++i) {
        auto cur = points[i];
        auto next = points[(i + 1) % size];
        if (is_on_line(p, cur, next) == 1) {
            return true;
        }
        //! exclude the case that point p is on the extension of the segment
        if (abs(cur.second - next.second) < eps) {
            continue;
        }
        // only the vertex with the larger y coordinate is counted
        if (is_on_line(cur, p, end) == 1) {
            if (cur.second > next.second) {
                ++cnt;
            }
        } else if (is_on_line(next, p, end) == 1) {
            if (next.second > cur.second) {
                ++cnt;
            }
        } else if (is_line_intersection(cur, next, p, end) == 1) {
            ++cnt;
        }
    }
    return (cnt % 2);
}

//* return true if segment (src, dst) is fully inside the polygon
bool poly::inside(std::pair<double, double> src, std::pair<double, double> dst) {
    if (!inside(src) || !inside(dst)) {
        return false;
    }
    if (src == dst) {
        return inside(src);
    }
    int size = points.size();
    int x_dir = (dst.first >= src.first) ? 1 : -1;
    int y_dir = (dst.second >= src.second) ? 1 : -1;
    std::vector<std::pair<double, double>> intersect_points;
    intersect_points.push_back(src);
    intersect_points.push_back(dst);
    for (int i = 0; i < size; ++i) {
        auto cur = points[i];
        auto next = points[(i + 1) % size];
        int intersection = is_line_intersection(src, dst, cur, next);
        if (intersection == 1 || intersection == -1) {
            intersect_points.push_back(get_intersection(src, dst, cur, next));
        } else if (intersection == 0) {
            continue;
        } else {
            if (is_on_line(cur, src, dst) == 1) {
                intersect_points.push_back(cur);
            }
            if (is_on_line(next, src, dst) == 1) {
                intersect_points.push_back(next);
            }
        }
    }
    auto f = [x_dir, y_dir](std::pair<double, double> p1, std::pair<double, double> p2) {
        if (x_dir == 1 && y_dir == 1) {
            if (p1.first != p2.first) {
                return (p1.first < p2.first);
            } else {
                return (p1.second < p2.second);
            }
        } else if (x_dir == 1 && y_dir == -1) {
            if (p1.first != p2.first) {
                return (p1.first < p2.first);
            } else {
                return (p1.second > p2.second);
            }
        } else if (x_dir == -1 && y_dir == 1) {
            if (p1.first != p2.first) {
                return (p1.first > p2.first);
            } else {
                return (p1.second < p2.second);
            }
        } else if (x_dir == -1 && y_dir == -1) {
            if (p1.first != p2.first) {
                return (p1.first > p2.first);
            } else {
                return (p1.second > p2.second);
            }
        }
        return false;
    };
    std::sort(intersect_points.begin(), intersect_points.end(), f);
    auto prev = intersect_points[0];
    for (int i = 1; i < intersect_points.size(); ++i) {
        auto cur = intersect_points[i];
        if (!inside({(prev.first + cur.first) / 2.0, (prev.second + cur.second) / 2.0})) {
            return false;
        }
        prev = cur;
    }
    return true;
}

//* return the intersection points of a line from src to dst with every edge of the polygon
std::set<std::pair<double, double>> poly::intersect(std::pair<double, double> src, std::pair<double, double> dst) {
    double threshold;
    const double eps = 1e-6;
    std::set<std::pair<double, double>> res;
    if (src == dst || !inside(src) || !inside(dst)) {
        return res;
    }
    // set the threshold, which means the end point of a "infinite long" segment from src to dst
    // use the parameter form of a line (l = p1 + t * (p2 - p1)), threshold actually is "t"
    if (src.first == dst.first) {
        if (dst.second > src.second) {
            threshold = (max_y - src.second) / (dst.second - src.second) + 2.0;
        } else {
            threshold = (min_y - src.second) / (dst.second - src.second) + 2.0;
        }
    } else if (src.second == dst.second) {
        if (dst.first > src.first) {
            threshold = (max_x - src.first) / (dst.first - src.first) + 2.0;
        } else {
            threshold = (min_x - src.first) / (dst.first - src.first) + 2.0;
        }
    } else {
        double t_x_max = (max_x - src.first) / (dst.first - src.first) + 2.0;
        double t_x_min = (min_x - src.first) / (dst.first - src.first) + 2.0;
        double t_y_max = (max_y - src.second) / (dst.second - src.second) + 2.0;
        double t_y_min = (min_y - src.second) / (dst.second - src.second) + 2.0;
        if (src.first < dst.first && src.second < dst.second) {
            threshold = std::max(t_x_max, t_y_max);
        } else if (src.first < dst.first && src.second > dst.second) {
            threshold = std::max(t_x_max, t_y_min);
        } else if (src.first > dst.first && src.second < dst.second) {
            threshold = std::max(t_x_min, t_y_max);
        } else if (src.first > dst.first && src.second > dst.second) {
            threshold = std::max(t_x_min, t_y_min);
        }
    }
    
    int size = this->points.size();
    for (int i = 0; i < size; ++i) {
        auto cur = points[i];
        auto next = points[(i + 1) % size];
        std::pair inf = {src.first + threshold * (dst.first - src.first), src.second + threshold * (dst.second - src.second)};
        auto intersection = is_line_intersection(src, inf, cur, next);
        if (intersection == 1) {
            // if regular intersection, then the line will be outside the polygon, break it.
            std::vector<double> src_to_dst = {dst.first - src.first, dst.second - src.second};
            std::vector<double> src_to_cur = {cur.first - src.first, cur.second - src.second};
            std::vector<double> src_to_next = {next.first - src.first, next.second - src.second};
            double lambda = abs((src_to_cur[0] * src_to_dst[1] - src_to_cur[1] * src_to_dst[0]) / 
                            (src_to_next[0] * src_to_dst[1] - src_to_next[1] * src_to_dst[0]));
            std::pair<double, double> intersection_point = {cur.first + lambda * (next.first - cur.first) / (1 + lambda), 
                                                            cur.second + lambda * (next.second - cur.second) / (1 + lambda)};
            if (inside(src, intersection_point)) {
                res.emplace(intersection_point);
            }
        } else if (intersection == -1) {
            auto intersection_point = get_intersection(src, inf, cur, next);
            if (inside(src, intersection_point)) {
                res.emplace(intersection_point);
            }
        } else if (intersection == -2) {
            // count the "t" of cur and next, if greater than 1, then check inside.
            // if inside, we can push it into the set
            double t_cur = (cur.first - src.first) / (dst.first - src.first);
            double t_next = (next.first - src.first) / (dst.first - src.first);
            if (t_cur > 1) {
                if (inside(src, cur)) {
                    res.emplace(cur);
                }
            }
            if (t_next > 1) {
                if (inside(src, next)) {
                    res.emplace(next);
                }
            }
        }
    }
    return res;
}

//* get all the visible vertices standing at src
//! vertices should be in vertex set
std::set<std::pair<double, double>> poly::visible(std::pair<double, double> src) {
    std::set<std::pair<double, double>> res;
    int size = points.size();
    for (int i = 0; i < size; ++i) {
        if (this->flag) {
            if (inside_obstacle(src, points[i])) {
                res.emplace(points[i]);
            }
        } else {
            if (inside(src, points[i])) {
                res.emplace(points[i]);
            }
        }
    }

    if (flag) {
        for (int i = 0; i < obstacle.size(); ++i) {
            if (inside_obstacle(src, obstacle[i])) {
                res.emplace(obstacle[i]);
            }
        }
    }

    return res;
}

//* return the union polygon of vc1 and vc2
return_value poly::polygon_union(std::vector<std::pair<double, double>> vc1, std::vector<std::pair<double, double>> vc2) { 
    return_value r;
    std::vector<std::vector<std::pair<double, double>>> res;
    Polygon poly1, poly2;
    double area = 0.0;

    std::vector<Point> points1, points2;
    for (auto&& [x, y] : vc1) {
        points1.emplace_back(x, y);
        // std::cout << x << " " << y << std::endl;
    }

    for (auto&& [x, y] : vc2) {
        points2.emplace_back(x, y);
        // std::cout << x << " " << y << std::endl;
    }

    bg::assign_points(poly1, points1);
    bg::assign_points(poly2, points2);
    //! 对于非凸多边形，不确定是否需要correct来对点进行排序
    bg::correct(poly1);
    bg::correct(poly2);
    // bg::buffer(poly1, poly1, 0.0);

    std::vector<Polygon> intersection;
    bg::union_(poly1, poly2, intersection);
    // std::cout << "intersection size: " << intersection.size() << std::endl;
    if (intersection.empty()) {
        return r;
    }

    for (auto&& poly : intersection) {
        area += bg::area(poly);
        std::vector<std::pair<double, double>> temp;
        for (auto&& p : poly.outer()) {
            temp.emplace_back(p.x(), p.y());
            // std::cout << p.x() << " " << p.y() << std::endl;
        }
        temp.pop_back();
        res.emplace_back(temp);
    }
    r.polygon = res;
    r.area = area;

    return r;
}

//* count the area of polygon specified by edge set vc
double poly::area_helper(std::vector<std::pair<double, double>> vc) {
    double res = 0.0;
    int size = vc.size();
    if (!size) {
        return 0.0;
    }
    for (int i = 0; i < size; ++i) {
        auto cur = vc[i];
        auto next = vc[(i + 1) % size];
        res += (cur.first * next.second - cur.second * next.first) / 2.0;
    }

    return abs(res);
}

//* reorder vertices in st
std::vector<std::pair<double, double>> poly::sort_vertex(std::set<std::pair<double, double>> st) {  
    // use a lambda function to get the possible vertex on a line 
    // iterate through all edges
    std::vector<std::pair<double, double>> res;
    //std::unordered_set<std::pair<double, double>> s;
    int size = points.size();

    for (int i = 0; i < size; ++i) {
        auto cur = this->points[i];
        auto next = points[(i + 1) % size];

        if (st.contains(cur)) {
            res.emplace_back(cur);
            st.erase(cur);
        }

        std::vector<std::pair<double, double>> temp;
        auto f = [this, i, size, cur, next] (std::pair<double, double> p) {
            return (is_on_line(p, cur, next) == 1);
        };
        std::copy_if(st.begin(), st.end(), std::back_inserter(temp), f);

        if (temp.size()) {
            auto it = std::find(temp.begin(), temp.end(), cur);
            if (it != temp.end()) {
                temp.erase(it);
            }
            it = std::find(temp.begin(), temp.end(), next);
            if (it != temp.end()) {
                temp.erase(it);
            }
            std::sort(temp.begin(), temp.end(), [cur, next](std::pair<double, double> p1, std::pair<double, double> p2) {
                double t1 = (p1.first - cur.first) / (next.first - cur.first);
                double t2 = (p2.first - cur.first) / (next.first - cur.first);
                return t1 < t2;
            });
            for (auto&& i : temp) {
                res.emplace_back(i);
                st.erase(i);
            }
        }
    }
    return res;
}

//* return all the vertices that can be seen at point src
//! vertices are not restricted to be inside the vertex sets
std::vector<std::pair<double, double>> poly::get_vertex_set(std::pair<double, double> src) {
    auto vis = visible(src);
    std::set<std::pair<double, double>> res;

    if (flag) {
        for (auto&& i : vis) {
            res.merge(intersect_obstacle(src, i));
        }
        return sort_vertex_obstacle(res, src);
    } else {
        for (auto&& i : vis) {
            res.merge(intersect(src, i));
        }
        return sort_vertex(res);
    }
}

//* count the area of visible area at point src
//* high level interface
double poly::area(std::pair<double, double> src) {
    return area_helper(get_vertex_set(src));
}

//* get all the pixels that are inside the polygon
void poly::set_inside_set() {
    #pragma omp parallel num_threads(8)
    {
        #pragma omp for
        for (int i = min_x; i < max_x; ++i) {
            std::vector<std::pair<double, double>> temp;
            for (int j = min_y; j < max_y; ++j) {
                if (inside_obstacle({i, j})) {
                    temp.emplace_back(i, j);
                }
            }
            #pragma omp critical
            {
                for (int i = 0; i < temp.size(); ++i) {
                    vertex_inside.push_back(temp[i]);
                }
            }
        }
    }
}

//* high level interface
void poly::execute_one_camera() {
    auto f = [](double x, double y) {return x > y;};
    std::map<double, std::pair<double, double>, decltype(f)> m(f);
    
    #pragma omp parallel num_threads(8)
    {
        #pragma omp for
        for (int i = min_x; i < max_x; ++i) {
            std::map<double, std::pair<double, double>> temp;
            for (int j = min_y; j < max_y; ++j) {
                if (flag) {
                    if (!inside_obstacle({i, j})) {
                        continue;
                    }
                } else {
                    if (!inside({i, j})) {
                        continue;
                    }
                }
                temp[area({i, j})] = {i, j};
            }
            #pragma omp critical
            {
                m.merge(temp);
            }
        }
    }
    
    // non-parallel version
    // for (int i = min_x; i < max_x; ++i) {
    //     for (int j = min_y; j < max_y; ++j) {
    //         if (inside({i, j})) {
    //             m[area({i, j})] = {i, j};
    //             // std::cout << "current pos : " << i << " " << j << std::endl;
    //             // std::cout << "area: " << area({i, j}) << std::endl;
    //             // std::cout << "vertex set: " << std::endl;
    //             // for (auto&& [x, y] : get_vertex_set({i, j})) {
    //             //     std::cout << x << " " << y << std::endl;
    //             // }
    //             // std::cout << std::endl;
    //         }
    //     }
    // }

    auto it = m.begin();
    this->pos = it->second;
    this->area_covered = it->first;
    this->vertex_set = get_vertex_set(pos);
    // std::cout << "final pos: " << pos.first << " " << pos.second << std::endl;
    // std::cout << "final area: " << area_covered << std::endl;
    // std::cout << "final vertex set: " << std::endl;
    // for (auto&& [x, y] : vertex_set) {
    //     std::cout << x << " " << y << std::endl;
    // }
    return ;
}

//! theoretically practical, but too time-consuming
//TODO: check the intersection of two polygons and seek to optimize
void poly::execute_two_camera() {
    set_inside_set();
    int size = vertex_inside.size();
    double max_area = 0.0;
    std::random_device rd;
    std::mt19937 g(rd());
    std::uniform_int_distribution<int> dis(0, size - 1);
    // auto f = [](double x, double y) {return x > y;};
    // std::map<double, std::vector<std::pair<double, double>>, decltype(f)> m(f);

    #pragma omp parallel num_threads(8)
    {
        #pragma omp for 
        for (int v1 = 0; v1 < size; ++v1) {
            std::vector<int> rnd_index; // pruning
            for (int i = 0; i < 40; ++i) {
                auto cur = dis(g);
                while (cur == v1) {
                    cur = dis(g);
                }
                rnd_index.emplace_back(cur);
            }

            std::map<double, std::vector<std::pair<double, double>>> temp;
            double area = 0.0;
            std::vector<std::pair<double, double>> temp_pos;
            std::vector<std::vector<std::pair<double, double>>> temp_poly_set;
            for (int v2 = 0; v2 < 40; ++v2) {
                auto p1 = vertex_inside[v1];
                auto p2 = vertex_inside[rnd_index[v2]];
                std::cout << "index " << v1 << " and " << v2 << std::endl;
                auto p1_set = get_vertex_set(p1);
                auto p2_set = get_vertex_set(p2);
                
                for (auto&& [x, y] : p1_set) {
                    std::cout << x << " " << y << std::endl;
                }
                std::cout << std::endl;
                for (auto&& [x, y] : p2_set) {
                    std::cout << x << " " << y << std::endl;
                }

                auto intersection_vertex = polygon_union(p1_set, p2_set);
                std::cout << "intersection vertex in index " << v1 << " " << v2 << " has size: " << intersection_vertex.polygon.size() << std::endl;
                double cur_area = intersection_vertex.area;
                // for (auto i : intersection_vertex) {
                //     cur_area += area_helper(i);
                // }

                if (cur_area > area) {
                    area = cur_area;
                    temp_pos = {p1, p2};
                    temp_poly_set = intersection_vertex.polygon;
                }
                // double cur_area = area_helper(p1_set) + area_helper(p2_set) - area_helper(intersection_vertex);
                // std::cout << "cur area : " << cur_area << std::endl;
                // if (cur_area > area) {
                //     area = cur_area;
                //     temp_pos = {p1, p2};
                //     temp_vertex = intersection_vertex;
                // }
            }
            #pragma omp critical
            {
                if (area > this->area_covered) {
                    area_covered = area;
                    pos_in_pair = temp_pos;
                    polygon_union_set = temp_poly_set;
                }
            }
        }
    }

    // for (int i = 0; i < size; ++i) {
    //     for (int j = i + 1; j < size; ++j) {
    //         auto p1 = vertex_inside[i];
    //         auto p2 = vertex_inside[j];
    //         std::cout << "index " << i << " and " << j << std::endl;
    //         auto intersection_vertex = polygon_intersect(get_vertex_set(p1), get_vertex_set(p2));
    //         std::cout << "intersection vertex in index " << i << " " << j << " has size: " << intersection_vertex.size() << std::endl;
    //         auto area = area_helper(intersection_vertex);
    //         if (area > max_area) {
    //             max_area = area;
    //             this->pos_in_pair = {p1, p2};
    //             this->vertex_set = intersection_vertex;
    //         }
    //     }
    // }

    // auto it = m.begin();
    // this->area_covered = it->first;
    // this->pos_in_pair = it->second;
    // this->vertex_set = polygon_intersect(get_vertex_set(pos_in_pair[0]), get_vertex_set(pos_in_pair[1]));
}

//* check if the obstacle is fully inside the polygon
bool poly::is_valid_obstacle() {
    int size = obstacle.size();
    if (size < 3) {
        return false;
    }

    for (int i = 0; i < size; ++i) {
        auto cur = obstacle[i];
        auto next = obstacle[(i + 1) % size];
        if (cur == next) {
            return false;
        }
        if (!inside(cur, next)) {
            return false;
        }
        
    }
    return true;
}

//* vertex inside funtion for polygon with an obstacle inside
//* inside the polygon, not the obstacle
bool poly::inside_obstacle(std::pair<double, double> p) {
    if (!inside(p)) {
        return false;
    }
    poly temp(this->obstacle);
    if (temp.inside(p)) {
        if (temp.vertex_on_edge(p)) {
            return true;
        }
        return false;
    }
    return true;
}

//* intersect function for polygon with an obstacle inde
std::set<std::pair<double, double>> poly::intersect_obstacle(std::pair<double, double> src, std::pair<double, double> dst) {
    double threshold;
    const double eps = 1e-6;
    std::set<std::pair<double, double>> res;
    if (src == dst || !inside_obstacle(src) || !inside_obstacle(dst)) {
        return res;
    }
    // set the threshold, which means the end point of a "infinite long" segment from src to dst
    // use the parameter form of a line (l = p1 + t * (p2 - p1)), threshold actually is "t"
    if (src.first == dst.first) {
        if (dst.second > src.second) {
            threshold = (max_y - src.second) / (dst.second - src.second) + 2.0;
        } else {
            threshold = (min_y - src.second) / (dst.second - src.second) + 2.0;
        }
    } else if (src.second == dst.second) {
        if (dst.first > src.first) {
            threshold = (max_x - src.first) / (dst.first - src.first) + 2.0;
        } else {
            threshold = (min_x - src.first) / (dst.first - src.first) + 2.0;
        }
    } else {
        double t_x_max = (max_x - src.first) / (dst.first - src.first) + 2.0;
        double t_x_min = (min_x - src.first) / (dst.first - src.first) + 2.0;
        double t_y_max = (max_y - src.second) / (dst.second - src.second) + 2.0;
        double t_y_min = (min_y - src.second) / (dst.second - src.second) + 2.0;
        if (src.first < dst.first && src.second < dst.second) {
            threshold = std::max(t_x_max, t_y_max);
        } else if (src.first < dst.first && src.second > dst.second) {
            threshold = std::max(t_x_max, t_y_min);
        } else if (src.first > dst.first && src.second < dst.second) {
            threshold = std::max(t_x_min, t_y_max);
        } else if (src.first > dst.first && src.second > dst.second) {
            threshold = std::max(t_x_min, t_y_min);
        }
    }
    
    int size = this->points.size();
    std::pair inf = {src.first + threshold * (dst.first - src.first), src.second + threshold * (dst.second - src.second)};
    for (int i = 0; i < size; ++i) {
        auto cur = points[i];
        auto next = points[(i + 1) % size];
        auto intersection = is_line_intersection(src, inf, cur, next);
        if (intersection == 1) {
            // if regular intersection, then the line will be outside the polygon, break it.
            std::vector<double> src_to_dst = {dst.first - src.first, dst.second - src.second};
            std::vector<double> src_to_cur = {cur.first - src.first, cur.second - src.second};
            std::vector<double> src_to_next = {next.first - src.first, next.second - src.second};
            double lambda = abs((src_to_cur[0] * src_to_dst[1] - src_to_cur[1] * src_to_dst[0]) / 
                            (src_to_next[0] * src_to_dst[1] - src_to_next[1] * src_to_dst[0]));
            std::pair<double, double> intersection_point = {cur.first + lambda * (next.first - cur.first) / (1 + lambda), 
                                                            cur.second + lambda * (next.second - cur.second) / (1 + lambda)};
            if (inside_obstacle(src, intersection_point)) {
                res.emplace(intersection_point);
            }
        } else if (intersection == -1) {
            auto intersection_point = get_intersection(src, inf, cur, next);
            if (inside_obstacle(src, intersection_point)) {
                res.emplace(intersection_point);
            }
        } else if (intersection == -2) {
            // count the "t" of cur and next, if greater than 1, then check inside.
            // if inside, we can push it into the set
            double t_cur = (cur.first - src.first) / (dst.first - src.first);
            double t_next = (next.first - src.first) / (dst.first - src.first);
            if (t_cur > 1) {
                if (inside_obstacle(src, cur)) {
                    res.emplace(cur);
                }
            }
            if (t_next > 1) {
                if (inside_obstacle(src, next)) {
                    res.emplace(next);
                }
            }
        }
    }

    int size_obstacle = obstacle.size();
    for (int i = 0; i < size_obstacle; ++i) {
        auto cur = obstacle[i];
        auto next = obstacle[(i + 1) % size_obstacle];
        auto intersection = is_line_intersection(src, inf, cur, next);
        if (intersection == 1) {
            // if regular intersection, then the line will be outside the polygon, break it.
            std::vector<double> src_to_dst = {dst.first - src.first, dst.second - src.second};
            std::vector<double> src_to_cur = {cur.first - src.first, cur.second - src.second};
            std::vector<double> src_to_next = {next.first - src.first, next.second - src.second};
            double lambda = abs((src_to_cur[0] * src_to_dst[1] - src_to_cur[1] * src_to_dst[0]) / 
                            (src_to_next[0] * src_to_dst[1] - src_to_next[1] * src_to_dst[0]));
            std::pair<double, double> intersection_point = {cur.first + lambda * (next.first - cur.first) / (1 + lambda), 
                                                            cur.second + lambda * (next.second - cur.second) / (1 + lambda)};
            if (inside_obstacle(src, intersection_point)) {
                res.emplace(intersection_point);
            }
        } else if (intersection == -1) {
            auto intersection_point = get_intersection(src, inf, cur, next);
            if (inside_obstacle(src, intersection_point)) {
                res.emplace(intersection_point);
            }
        } else if (intersection == -2) {
            // count the "t" of cur and next, if greater than 1, then check inside.
            // if inside, we can push it into the set
            double t_cur = (cur.first - src.first) / (dst.first - src.first);
            double t_next = (next.first - src.first) / (dst.first - src.first);
            if (t_cur > 1) {
                if (inside_obstacle(src, cur)) {
                    res.emplace(cur);
                }
            }
            if (t_next > 1) {
                if (inside_obstacle(src, next)) {
                    res.emplace(next);
                }
            }
        }
    }
    return res;
}

//* segment inside function for polygon with an obstacle inside
bool poly::inside_obstacle(std::pair<double, double> src, std::pair<double, double> dst) {
    if (!inside_obstacle(src) || !inside_obstacle(dst)) {
        return false;
    }
    if (src == dst) {
        return inside_obstacle(src);
    }
    int size = points.size();
    int x_dir = (dst.first >= src.first) ? 1 : -1;
    int y_dir = (dst.second >= src.second) ? 1 : -1;
    std::vector<std::pair<double, double>> intersect_points;
    intersect_points.push_back(src);
    intersect_points.push_back(dst);
    for (int i = 0; i < size; ++i) {
        auto cur = points[i];
        auto next = points[(i + 1) % size];
        int intersection = is_line_intersection(src, dst, cur, next);
        if (intersection == 1 || intersection == -1) {
            intersect_points.push_back(get_intersection(src, dst, cur, next));
        } else if (intersection == 0) {
            continue;
        } else {
            if (is_on_line(cur, src, dst) == 1) {
                intersect_points.push_back(cur);
            }
            if (is_on_line(next, src, dst) == 1) {
                intersect_points.push_back(next);
            }
        }
    }

    int size_obstacle = obstacle.size();
    for (int i = 0; i < size_obstacle; ++i) {
        auto cur = obstacle[i];
        auto next = obstacle[(i + 1) % size_obstacle];
        int intersection = is_line_intersection(src, dst, cur, next);
        if (intersection == 1 || intersection == -1) {
            intersect_points.push_back(get_intersection(src, dst, cur, next));
        } else if (intersection == 0) {
            continue;
        } else {
            if (is_on_line(cur, src, dst) == 1) {
                intersect_points.push_back(cur);
            }
            if (is_on_line(next, src, dst) == 1) {
                intersect_points.push_back(next);
            }
        }
    }

    auto f = [x_dir, y_dir](std::pair<double, double> p1, std::pair<double, double> p2) {
        if (x_dir == 1 && y_dir == 1) {
            if (p1.first != p2.first) {
                return (p1.first < p2.first);
            } else {
                return (p1.second < p2.second);
            }
        } else if (x_dir == 1 && y_dir == -1) {
            if (p1.first != p2.first) {
                return (p1.first < p2.first);
            } else {
                return (p1.second > p2.second);
            }
        } else if (x_dir == -1 && y_dir == 1) {
            if (p1.first != p2.first) {
                return (p1.first > p2.first);
            } else {
                return (p1.second < p2.second);
            }
        } else if (x_dir == -1 && y_dir == -1) {
            if (p1.first != p2.first) {
                return (p1.first > p2.first);
            } else {
                return (p1.second > p2.second);
            }
        }
        return false;
    };
    std::sort(intersect_points.begin(), intersect_points.end(), f);
    auto prev = intersect_points[0];
    for (int i = 1; i < intersect_points.size(); ++i) {
        auto cur = intersect_points[i];
        if (!inside_obstacle({(prev.first + cur.first) / 2.0, (prev.second + cur.second) / 2.0})) {
            return false;
        }
        prev = cur;
    }
    return true;
}

//* check if p is in the vertex set
bool poly::is_vertex(std::pair<double, double> p) {
    int size = points.size();
    for (int i = 0; i < size; ++i) {
        if (abs(points[i].first - p.first) < 1e-6 && abs(points[i].second - p.second) < 1e-6) {
            return true;
        }
    }

    if (flag) {
        for (int i = 0; i < obstacle.size(); ++i) {
            if (abs(obstacle[i].first - p.first) < 1e-6 && abs(obstacle[i].second - p.second) < 1e-6) {
                return true;
            }
        }
    }

    return false;
}

//* reorder function for polygon with an obstacle inside
std::vector<std::pair<double, double>> poly::sort_vertex_obstacle(std::set<std::pair<double, double>> st, std::pair<double, double> src) {
    std::vector<std::pair<double, double>> res;
    std::deque<std::pair<double, double>> dq;
    bool flag = is_vertex(src);
    if (!flag) {
        st.erase(src);
    }
    for (auto&& i : st) {
        dq.emplace_back(i);
    }
    auto f = [src](std::pair<double, double> p1, std::pair<double, double> p2) {
        double t1 = atan2(p1.second - src.second, p1.first - src.first);
        double t2 = atan2(p2.second - src.second, p2.first - src.first);
        if (abs(t1 - t2) > 1e-6) {
            return t1 < t2;
        } else {
            return ((p1.first - src.first) * (p1.first - src.first) + (p1.second - src.second) * (p1.second - src.second)) < 
                    ((p2.first - src.first) * (p2.first - src.first) + (p2.second - src.second) * (p2.second - src.second));
        }
    };
    std::sort(dq.begin(), dq.end(), f);
    double prev_angle = atan2(dq[0].second - src.second, dq[0].first - src.first);
    std::vector<std::pair<double, double>> temp_vc;
    std::vector<std::vector<std::pair<double, double>>> vc;
    if (flag) {
        vc.push_back({src});
    }
    while (!dq.empty()) {
        auto cur = dq.front();
        dq.pop_front();
        double cur_angle = atan2(cur.second - src.second, cur.first - src.first);
        if (abs(cur_angle - prev_angle) < 1e-6) {
            temp_vc.emplace_back(cur);
            continue;
        } else {
            vc.emplace_back(temp_vc);
            temp_vc.clear();
            temp_vc.emplace_back(cur);
            prev_angle = cur_angle;
        }
    }
    if (!temp_vc.empty()) {
        vc.emplace_back(temp_vc);
    }

    int size = vc.size();
    for (int i = 0; i < size; ++i) {
        auto cur = vc[i];
        auto next = vc[(i + 1) % size];
        auto cur_front = cur.front();
        auto cur_back = cur.back();
        auto next_front = next.front();
        auto next_back = next.back();
        if (is_valid_edge(cur_back, next_front)) {
            continue;
        } else if (is_valid_edge(cur_back, next_back)) {
            std::reverse(next.begin(), next.end());
        } else if(is_valid_edge(cur_front, next_front)) {
            std::reverse(cur.begin(), cur.end());
        } else if (is_valid_edge(cur_front, next_back)) {
            std::reverse(cur.begin(), cur.end());
            std::reverse(next.begin(), next.end());
        }
        vc[i] = cur;
        vc[(i + 1) % size] = next;
    }
    for (auto&& i : vc) {
        for (auto&& j : i) {
            res.emplace_back(j);
        }
    }
    return res;
}