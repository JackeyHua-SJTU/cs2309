#include "poly.h"

poly::poly(std::vector<std::pair<double, double>> vc) : points(vc), min_x(INT_MAX), max_x(INT_MIN), min_y(INT_MAX), max_y(INT_MIN) {
    for (auto&& [x, y] : this->points) {
        min_x = std::min(min_x, x);
        max_x = std::max(max_x, x);
        min_y = std::min(min_y, y);
        max_y = std::max(max_y, y);
    }
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

// TODO: verify the correctness of this function
std::set<std::pair<double, double>> poly::intersect(std::pair<double, double> src, std::pair<double, double> dst) {
    double threshold;
    const double eps = 1e-6;
    std::set<std::pair<double, double>> res;
    if (src == dst) {
        return res;
    }
    if (src.first == dst.first) {
        if (dst.second > src.second) {
            threshold = (max_y - src.second) / (dst.second - src.second) + 2.0;
        } else {
            threshold = (min_y - src.second) / (dst.second - src.second) + 2.0;
        }
    } 
    if (src.second == dst.second) {
        if (dst.first > src.first) {
            threshold = (max_x - src.first) / (dst.first - src.first) + 2.0;
        } else {
            threshold = (min_x - src.first) / (dst.first - src.first) + 2.0;
        }
    }
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
    
    int size = this->points.size();
    for (int i = 0; i < size; ++i) {
        auto cur = points[i];
        auto next = points[(i + 1) % size];
        std::pair inf = {src.first + threshold * (dst.first - src.first), src.second + threshold * (dst.second - src.second)};
        auto intersection = is_line_intersection(src, inf, cur, next);
        if (intersection == 1) {
            std::vector<double> src_to_dst = {dst.first - src.first, dst.second - src.second};
            std::vector<double> src_to_cur = {cur.first - src.first, cur.second - src.second};
            std::vector<double> src_to_next = {next.first - src.first, next.second - src.second};
            double lambda = abs((src_to_cur[0] * src_to_dst[1] - src_to_cur[1] * src_to_dst[0]) / 
                            (src_to_next[0] * src_to_dst[1] - src_to_next[1] * src_to_dst[0]));
            res.emplace(cur.first + lambda * (next.first - cur.first) / (1 + lambda), 
                            cur.second + lambda * (next.second - cur.second) / (1 + lambda));
        } else if (intersection == -1) {
            if (is_on_line(cur, src, inf) == 1) {
                res.emplace(cur);
            }
            if (is_on_line(next, src, inf) == 1) {
                res.emplace(next);
            }
        } else if (intersection == -2) {
            std::vector<double> src_to_dst = {dst.first - src.first, dst.second - src.second};
            std::vector<double> src_to_cur = {cur.first - src.first, cur.second - src.second};
            std::vector<double> src_to_next = {next.first - src.first, next.second - src.second};
            int flag_cur = (src_to_cur[0] * src_to_dst[0] + src_to_cur[1] * src_to_dst[1] >= 0) ? 1 : -1;
            int flag_next = (src_to_next[0] * src_to_dst[0] + src_to_next[1] * src_to_dst[1] >= 0) ? 1 : -1;
            if (flag_cur * (src_to_cur[0] * src_to_cur[0] + src_to_cur[1] * src_to_cur[1]) < 
                flag_next * (src_to_next[0] * src_to_next[0] + src_to_next[1] * src_to_next[1])) {
                res.emplace(next);
            } else {
                res.emplace(cur);
            }
        }
    }
    return res;

}