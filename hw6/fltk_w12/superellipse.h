#ifndef _SUPERELLIPSE_H_
#define _SUPERELLIPSE_H_

#include <vector>
#include <FL/Fl_Window.H>
#include <FL/fl_draw.H>

class superellipse : public Fl_Window {
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
    superellipse(double a, double b, double m, double n, int N, int k, int width, int height);
    std::vector<std::pair<double, double>> getPoints() { return this->points; }

    // TODO : Modify drawing logic, make points alternate over and below x axis (bipartite)
    void draw() override;   // draw the superellipse    
};

#endif