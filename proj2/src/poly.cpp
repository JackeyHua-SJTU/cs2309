#include "poly.h"

poly::poly(std::vector<std::pair<double, double>> vc) : points(vc), min_x(INT_MAX), max_x(INT_MIN), min_y(INT_MAX), max_y(INT_MIN) {
    for (auto&& [x, y] : this->points) {
        min_x = std::min(min_x, x);
        max_x = std::max(max_x, x);
        min_y = std::min(min_y, y);
        max_y = std::max(max_y, y);
    }
}

bool poly::is_line_intersection(std::pair<double, double> l1_src, std::pair<double, double> l1_dst, std::pair<double, double> l2_src, std::pair<double, double> l2_dst) {
    // regular intersection
    std::vector<double> l1 = {l1_dst.first - l1_src.first, l1_dst.second - l1_src.second};
    std::vector<double> l2 = {l2_dst.first - l2_src.first, l2_dst.second - l2_src.second};
    double cross_l1_l2_src = l1[0] * (l2_src.second - l1_src.second) - l1[1] * (l2_src.first - l1_src.first);
    double cross_l1_l2_dst = l1[0] * (l2_dst.second - l1_src.second) - l1[1] * (l2_dst.first - l1_src.first);
    double cross_l2_l1_src = l2[0] * (l1_src.second - l2_src.second) - l2[1] * (l1_src.first - l2_src.first);
    double cross_l2_l1_dst = l2[0] * (l1_dst.second - l2_src.second) - l2[1] * (l1_dst.first - l2_src.first);
    if ((cross_l1_l2_src * cross_l1_l2_dst < 0) && (cross_l2_l1_src * cross_l2_l1_dst < 0)) {
        return true;
    }
    // irregular intersection
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
        return false;
    }
    std::vector<double> cur_to_start = {start.first - cur.first, start.second - cur.second};
    std::vector<double> cur_to_end = {end.first - cur.first, end.second - cur.second};
    if (cur_to_start[0] * cur_to_end[0] + cur_to_start[1] * cur_to_end[1] <= 0) {
        return true;
    }
    return false;
}

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
        } else if (is_line_intersection(cur, next, p, end)) {
            ++cnt;
        }
    }
    return (cnt % 2);
}