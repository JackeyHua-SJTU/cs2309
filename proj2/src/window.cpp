#include "window.h"
#include <FL/fl_draw.H>
#include <iostream>
#include <iomanip>

window::window(int w, int h, std::vector<std::pair<double, double>> points, const char *title) : Fl_Window(w, h, title) {
    this->points = points;
    p = poly(points);
    p.execute_one_camera();
    std::cout << std::fixed << std::setprecision(4) << "area: " << p.area_covered << std::endl;
    std::cout << "pos : " << p.pos.first << " " << p.pos.second << std::endl;
    std::cout << "vertex set: " << std::endl;
    for (auto&& [x, y] : p.vertex_set) {
        std::cout << x << " " << y << std::endl;
    }
}

void window::draw() {
    Fl_Window::draw();
    fl_color(FL_BLACK);

    int size = points.size();

    for (auto&& [x, y] : points) {
        fl_color(FL_RED);
        fl_circle(x, y, 2);
    }

    for (int i = 0; i < points.size(); i++) {
        fl_line(points[i].first, points[i].second, points[(i + 1) % points.size()].first, points[(i + 1) % points.size()].second);
    }

    fl_color(FL_BLUE);
    fl_begin_polygon();
    for (auto&& [x, y] : p.vertex_set) {
        fl_vertex(x, y);
    }
    fl_end_polygon();

    fl_color(FL_GREEN);
    fl_circle(p.pos.first, p.pos.second, 2);

}