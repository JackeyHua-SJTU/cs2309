#ifndef _SUPERELLIPSE_H_
#define _SUPERELLIPSE_H_

#include <vector>

class superellipse {
private:
    // (x/a)^m + (y/b)^n = 1
    double a;
    double b;
    double m;
    double n;
    int N;   // select N points on the curve
    int k;   // each point connects to k points
    std::vector<std::pair<double, double>> points; 

public:
    superellipse(double a, double b, double m, double n, int N, int k);
    std::vector<std::pair<double, double>> getPoints() { return this->points; }
};

#endif