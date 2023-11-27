#include "star.h"
#include <stdexcept>
#include <vector>
#include <cmath>
#include <FL/fl_draw.H>

star::star(int num, int width, int height, const char* s) : Fl_Window(width, height, s) {
    this->number = num;
    resizable(this);
}

void star::valid() {
    if (this->number <= 2) {
        throw std::runtime_error("Number of stars must be greater than 2.");
    }
}

void star::draw() {
    this->valid();
    Fl_Window::draw();
    
    int w = this->w();
    int h = this->h();
    double radius_large = (w > h) ? h / 3.0 : w / 3.0;
    double unit_angle = 2 * M_PI / this->number;
    double radius_small = radius_large / (2 * cos(unit_angle / 2));

    std::vector<std::pair<int, int>> points1;
    std::vector<std::pair<int, int>> points2;
    std::vector<Fl_Color> colors{FL_RED, FL_GREEN, FL_DARK_BLUE, FL_DARK_MAGENTA, FL_DARK_CYAN};
    for (int i = 0; i < this->number; i++) {
        int x1 = w / 2 + radius_large * cos(unit_angle * i);
        int y1 = h / 2 - radius_large * sin(unit_angle * i);
        int x2 = w / 2 + radius_small * cos(unit_angle * i + unit_angle / 2);
        int y2 = h / 2 - radius_small * sin(unit_angle * i + unit_angle / 2);
        points1.emplace_back(x1, y1);
        points2.emplace_back(x2, y2);
    }

    int s = points1.size();
    int color = 0;
    int thickness = 3;
    for (int i = 0 ; i < s; ++i) {
        fl_color(colors[color]);
        color = (color + 1) % colors.size();
        for (int j = 0; j < thickness; ++j) {
            fl_line(points1[i].first, points1[i].second, points2[i].first, points2[i].second);
            fl_line(points2[i].first, points2[i].second, points1[(i + 1) % s].first, points1[(i + 1) % s].second);
        }       
    }
    color = 2;
    for (int i = 0; i < s; ++i) {
        fl_color(colors[color]);
        color = (color + 1) % colors.size();
        fl_begin_polygon();
        fl_vertex(w / 2, h / 2);
        fl_vertex(points1[i].first, points1[i].second);
        fl_vertex(points2[i].first, points2[i].second);
        fl_vertex(points1[(i + 1) % s].first, points1[(i + 1) % s].second);
        fl_end_polygon();
    }

}