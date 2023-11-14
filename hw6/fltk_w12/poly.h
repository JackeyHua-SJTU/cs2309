#ifndef _POLY_H_
#define _POLY_H_

#include <vector>

class poly {
private:
    std::vector<std::pair<double, double>> points;
    bool valid;

    bool isValidPoly(std::pair<double, double> p1, std::pair<double, double> p2, std::pair<double, double> q1, std::pair<double, double> q2);

    bool isLine();

public:
    poly(std::vector<std::pair<double, double>> vc);

    bool isValid() { return this->valid; }

};

#endif