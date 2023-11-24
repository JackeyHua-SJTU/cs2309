#ifndef _POLYGON_H_
#define _POLYGON_H_

#include <vector>


class Polygon {
private:

    void sort_points();

    double angle(std::pair<double, double> src, std::pair<double, double> dst);

    double count_area();

public:
    Polygon(std::vector<std::pair<double, double>> points);

    Polygon() = default;

    void sort() {
        sort_points();
    }

    double area() {
        return count_area();
    }

    std::vector<std::pair<double, double>> points;

};

#endif