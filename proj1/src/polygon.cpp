#include <algorithm>
#include <cmath>
#include "polygon.hpp"

void Polygon::sort_points() {
    // 使用扫描线算法对多边形的顶点进行排序
    auto extreme = Polygon::points[0];
    for (auto&& p : Polygon::points) {
        if (p.first > extreme.first) {
            extreme = p;
        } else if (p.first == extreme.first && p.second < extreme.second) {
            extreme = p;
        }
    }
    auto cmp = [=](std::pair<double, double> p1, std::pair<double, double> p2) {
        double angle1 = Polygon::angle(extreme, p1), angle2 = Polygon::angle(extreme, p2);
        if (angle1 != angle2) {
            return angle1 < angle2;
        } else {
            return p1.first * p1.first + p1.second * p1.second < p2.first * p2.first + p2.second * p2.second;
        }
    };
    std::sort(Polygon::points.begin(), Polygon::points.end(), cmp);
    
}

double Polygon::count_area() {
    // 计算多边形的面积
    double area = 0.0;
    for (int i = 0; i < Polygon::points.size() - 1; ++i) {
        auto p1 = Polygon::points[i], p2 = Polygon::points[i + 1];
        area += (p1.first * p2.second - p1.second * p2.first) / 2;
    }
    area += (Polygon::points.back().first * Polygon::points.front().second - Polygon::points.back().second * Polygon::points.front().first) / 2;
    return area;
}

double Polygon::angle(std::pair<double, double> src, std::pair<double, double> dst) {
    // 计算两个向量的夹角
    double x1 = src.first, y1 = src.second, x2 = dst.first, y2 = dst.second;
    double pi = std::acos(-1);
    if (x1 == x2) {
        if (y1 > y2) {
            return pi / 2.0;
        } else if (y1 < y2) { 
            return - pi / 2.0;
        } else {
            return - pi;
        }
    }
    return std::atan((y2 - y1) / (x2 - x1));
}

Polygon::Polygon(std::vector<std::pair<double, double>>&& points) {
    Polygon::points = points;
}