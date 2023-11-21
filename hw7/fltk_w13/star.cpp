#include "star.h"
#include <stdexcept>
#include <vector>
#include <cmath>
#include <FL/fl_draw.H>

star::star(int num, int width, int height) : Fl_Window(width, height) {
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
    int radis = (w > h) ? h / 3 : w / 3;

    std::vector<std::pair<int, int>> points;
    std::vector<Fl_Color> colors{FL_RED, FL_GREEN, FL_DARK_BLUE, FL_DARK_MAGENTA, FL_DARK_CYAN};
    for (int i = 0; i < this->number - 1; i++) {
        double angle = 2 * M_PI * i / (this->number - 1);
        int x = w / 2 + radis * cos(angle);
        int y = h / 2 - radis * sin(angle);
        points.emplace_back(x, y);
    }

    int s = points.size();
    int color = 0;
    int thickness = 3;
    for (int i = 0 ; i < s; ++i) {
        fl_color(colors[color]);
        color = (color + 1) % colors.size();
        for (int j = 0; j < thickness; ++j) {
            fl_line(w / 2, h / 2 + j, points[i].first, points[i].second + j);
            fl_line(points[i].first, points[i].second + j, points[(i + 1) % s].first, points[(i + 1) % s].second + j);
        }       
    }
    color = 2;
    for (int i = 0; i < s; ++i) {
        fl_color(colors[color]);
        color = (color + 1) % colors.size();
        fl_begin_polygon();
        fl_vertex(w / 2, h / 2);
        fl_vertex(points[i].first, points[i].second);
        fl_vertex(points[(i + 1) % s].first, points[(i + 1) % s].second);
        fl_end_polygon();
    }

}