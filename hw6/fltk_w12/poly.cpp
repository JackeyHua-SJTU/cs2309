#include "poly.h"

using std::pair;
using std::vector;

bool poly::isLine() {
    /**
     * @brief 
     * Check if the polygon is a line
     * 
    */

    // check if all the points are on the same line
    // if they are, then the cross product of any 2 vectors should be 0
    // let the first point be the origin
    // then the cross product of the other 2 points should be 0
    // (x1, y1) cross product (x2, y2) = x1 * y2 - x2 * y1
    int len = points.size();
    for (int i = 1; i < len - 1; ++i) {
        double x1 = points[i].first - points[0].first;
        double y1 = points[i].second - points[0].second;
        double x2 = points[i + 1].first - points[0].first;
        double y2 = points[i + 1].second - points[0].second;
        if (x1 * y2 - x2 * y1 != 0) return false;
    }
    return true;
}

bool poly::isValidPoly(pair<double, double> p1, pair<double, double> p2, pair<double, double> q1, pair<double, double> q2) {
    /**
     * @brief 
     * Given 2 lines, P1P2, Q1Q2, check if they intersect
     * Let A = P1P2 cross product P1Q1, B = P1P2 cross product P1Q2
     * Let C = Q1Q2 cross product Q1P1, D = Q1Q2 cross product Q1P2
     * 
     * if A * B < 0 and C * D < 0, then intersect
     * 
    */

    // P1P2 = (p2.first - p1.first, p2.second - p1.second)
    // P1Q1 = (q1.first - p1.first, q1.second - p1.second)
    // P1Q2 = (q2.first - p1.first, q2.second - p1.second)
    // Q1Q2 = (q2.first - q1.first, q2.second - q1.second)
    // Q1P1 = (p1.first - q1.first, p1.second - q1.second)
    // Q1P2 = (p2.first - q1.first, p2.second - q1.second)
    double A = (p2.first - p1.first) * (q1.second - p1.second) - (q1.first - p1.first) * (p2.second - p1.second);
    double B = (p2.first - p1.first) * (q2.second - p1.second) - (q2.first - p1.first) * (p2.second - p1.second);
    double C = (q2.first - q1.first) * (p1.second - q1.second) - (p1.first - q1.first) * (q2.second - q1.second);
    double D = (q2.first - q1.first) * (p2.second - q1.second) - (p2.first - q1.first) * (q2.second - q1.second);

    return (A * B < 0) && (C * D < 0);

}

poly::poly(vector<pair<double, double>> vc) {
    this->points = vc;

    if (isLine()) {
        valid = false;
        return;
    }

    // check if the polygon is valid
    int len = vc.size();
    for (int i = 0; i < len - 1; ++i) {
        // each iteration check if line (v[i], v[i + 1]) intersects with others
        for (int j = 0; j < len; ++j) {
            // check all the lines except (v[i], v[i + 1]), (v[i + 1], v[i + 2]), (v[i - 1], v[i])
            if (j == i || j == (i + 1) % len || j == (i - 1 + len) % len) continue;
            if (isValidPoly(vc[i], vc[i + 1], vc[j], vc[(j + 1) % len])) {
                valid = false;
                return;
            }
        }
    }
    valid = true;
}