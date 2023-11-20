#ifndef _POLY_H_
#define _POLY_H_

#include <vector>
#include <FL/Fl_Window.H>
#include <FL/fl_draw.H>

class poly : public Fl_Window {
private:
    std::vector<std::pair<double, double>> points;
    bool valid;

    bool isValidPoly(std::pair<double, double> p1, std::pair<double, double> p2, std::pair<double, double> q1, std::pair<double, double> q2);
    
    // bool isLine();

public:
    poly(std::vector<std::pair<double, double>> vc, int width, int height);

    bool isValid() { return this->valid; }

    void draw() override;   // draw the polygon

};

#endif