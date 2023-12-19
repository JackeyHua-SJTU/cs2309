#include "poly.h"
#include <unordered_set>
#include <algorithm>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>

namespace bg = boost::geometry;

typedef bg::model::d2::point_xy<double> Point;
typedef bg::model::polygon<Point> Polygon;

poly::poly(std::vector<std::pair<double, double>> vc) : points(vc), min_x(INT_MAX), max_x(INT_MIN), min_y(INT_MAX), max_y(INT_MIN) {
    for (auto&& [x, y] : this->points) {
        min_x = std::min(min_x, x);
        max_x = std::max(max_x, x);
        min_y = std::min(min_y, y);
        max_y = std::max(max_y, y);
    }
}

//* cross product of two vectors
double poly::cross(std::vector<double> v1, std::vector<double> v2) {
    return v1[0] * v2[1] - v1[1] * v2[0];
}

//* return 1 if regular intersection, -2 if irregular intersection (parallel)
//* return -1 if irregular intersection (colinear), 0 if no intersection
//! intersection of two SEGMENTS
int poly::is_line_intersection(std::pair<double, double> l1_src, std::pair<double, double> l1_dst, std::pair<double, double> l2_src, std::pair<double, double> l2_dst) {
    // regular intersection
    std::vector<double> l1 = {l1_dst.first - l1_src.first, l1_dst.second - l1_src.second};
    std::vector<double> l2 = {l2_dst.first - l2_src.first, l2_dst.second - l2_src.second};
    double cross_l1_l2_src = l1[0] * (l2_src.second - l1_src.second) - l1[1] * (l2_src.first - l1_src.first);
    double cross_l1_l2_dst = l1[0] * (l2_dst.second - l1_src.second) - l1[1] * (l2_dst.first - l1_src.first);
    double cross_l2_l1_src = l2[0] * (l1_src.second - l2_src.second) - l2[1] * (l1_src.first - l2_src.first);
    double cross_l2_l1_dst = l2[0] * (l1_dst.second - l2_src.second) - l2[1] * (l1_dst.first - l2_src.first);
    if ((cross_l1_l2_src * cross_l1_l2_dst < 0) && (cross_l2_l1_src * cross_l2_l1_dst < 0)) {
        return 1;
    }
    // irregular intersection
    // colinear
    if (!cross_l1_l2_dst && !cross_l1_l2_src && !cross_l2_l1_dst && !cross_l2_l1_src) {
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
    if (!cross_l1_l2_src || !cross_l1_l2_dst) {
        start = l1_src;
        end = l1_dst;
        if (!cross_l1_l2_src) {
            cur = l2_src;
        } else {
            cur = l2_dst;
        }
    } else if (!cross_l2_l1_src || !cross_l2_l1_dst) {
        start = l2_src;
        end = l2_dst;
        if (!cross_l2_l1_src) {
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

std::set<std::pair<double, double>> poly::visible(std::pair<double, double> src) {
    std::set<std::pair<double, double>> res;
    int size = points.size();
    for (int i = 0; i < size; ++i) {
        if (inside(src, points[i])) {
            res.emplace(points[i]);
        }
    }
    return res;
}

std::vector<std::pair<double, double>> poly::polygon_intersect(std::vector<std::pair<double, double>> vc1, std::vector<std::pair<double, double>> vc2) {
    std::vector<std::pair<double, double>> res;
    Polygon poly1, poly2;

    std::vector<Point> points1, points2;
    for (auto&& [x, y] : vc1) {
        points1.emplace_back(x, y);
    }
    for (auto&& [x, y] : vc2) {
        points2.emplace_back(x, y);
    }

    bg::assign_points(poly1, points1);
    bg::assign_points(poly2, points2);
    //! 对于非凸多边形，不确定是否需要correct来对点进行排序
    bg::correct(poly1);
    bg::correct(poly2);

    std::vector<Polygon> intersection;
    bg::intersection(poly1, poly2, intersection);
    Polygon inter = intersection[0];

    for (auto&& p : inter.outer()) {
        res.emplace_back(p.x(), p.y());
    }
    res.pop_back();
    return res;
}

double poly::area_helper(std::vector<std::pair<double, double>> vc) {
    double res = 0;
    int size = vc.size();
    for (int i = 0; i < size; ++i) {
        auto cur = vc[i];
        auto next = vc[(i + 1) % size];
        res += (cur.first * next.second - cur.second * next.first) / 2.0;
    }

    auto cur = vc[size - 1];
    auto next = vc[0];
    res += (cur.first * next.second - cur.second * next.first) / 2.0;

    return abs(res);
}

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

double poly::area(std::pair<double, double> src) {
    auto vis = visible(src);
    std::set<std::pair<double, double>> res;

    for (auto&& i : vis) {
        res.merge(intersect(src, i));
    }

    auto vc = sort_vertex(res);
    return area_helper(vc);
}