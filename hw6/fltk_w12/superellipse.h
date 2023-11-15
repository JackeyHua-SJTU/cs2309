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
    std::vector<std::pair<double, double>> points; 

public:
    superellipse(double a, double b, double m, double n, int N, int width, int height);
    std::vector<std::pair<double, double>> getPoints() { return this->points; }

    // mean distributed in x axis
    // y value alternates over and below x axis
    // like a bipartite graph
    void draw() override;   // draw the superellipse    
};

#endif