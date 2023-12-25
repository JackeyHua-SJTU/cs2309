#include "window.h"
#include <FL/fl_draw.H>
#include <iostream>
#include <iomanip>

window::window(int w, int h, std::vector<std::pair<double, double>> points, int op, const char *title) : Fl_Window(w, h, title) {
    this->points = points;
    this->operation = op;
    p = poly(points);

    if (op == 1) {
        p.execute_one_camera();
        std::cout << std::fixed << std::setprecision(6) << "area: " << p.area_covered << std::endl;
        std::cout << "pos : " << p.pos.first << " " << p.pos.second << std::endl;
        std::cout << "vertex set: " << std::endl;
        for (auto&& [x, y] : p.vertex_set) {
            std::cout << x << " " << y << std::endl;
        }
    } else if (op == 2) {
        p.execute_two_camera();
        std::cout << std::fixed << std::setprecision(6) << "area: " << p.area_covered << std::endl;
        std::cout << "pos : " << p.pos_in_pair[0].first << " " << p.pos_in_pair[0].second << std::endl;
        std::cout << "pos : " << p.pos_in_pair[1].first << " " << p.pos_in_pair[1].second << std::endl;
        for (auto&& i : p.polygon_union_set) {
            for (auto&& [x, y] : i) {
                std::cout << x << " " << y << std::endl;
            }
            std::cout << std::endl;
        }
    }
}

window::window(int w, int h, std::vector<std::pair<double, double>> points, std::vector<std::pair<double, double>> obs, int op, const char *title) : Fl_Window(w, h, title) {
    this->points = points;
    this->operation = op;
    p = poly(points, true);
    p.set_obstacle(obs);

    if (op == 1) {
        p.execute_one_camera();
        std::cout << std::fixed << std::setprecision(6) << "area: " << p.area_covered << std::endl;
        std::cout << "pos : " << p.pos.first << " " << p.pos.second << std::endl;
        std::cout << "vertex set: " << std::endl;
        for (auto&& [x, y] : p.vertex_set) {
            std::cout << x << " " << y << std::endl;
        }
    } else if (op == 2) {
        p.execute_two_camera();
        std::cout << std::fixed << std::setprecision(6) << "area: " << p.area_covered << std::endl;
        std::cout << "pos : " << p.pos_in_pair[0].first << " " << p.pos_in_pair[0].second << std::endl;
        std::cout << "pos : " << p.pos_in_pair[1].first << " " << p.pos_in_pair[1].second << std::endl;
        for (auto&& i : p.polygon_union_set) {
            for (auto&& [x, y] : i) {
                std::cout << x << " " << y << std::endl;
            }
            std::cout << std::endl;
        }
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
    if (this->operation == 1) {
        fl_begin_polygon();
        for (auto&& [x, y] : p.vertex_set) {
            fl_vertex(x, y);
        }
        fl_end_polygon();
    } else if (this->operation == 2) {
        for (auto&& i : p.polygon_union_set) {
            fl_begin_polygon();
            for (auto&& [x, y] : i) {
                fl_vertex(x, y);
            }
            fl_end_polygon();
        }
    }
    
    if (p.get_flag()) {
        fl_color(FL_BLACK);
        auto obs = p.get_obstacle();
        for (int i = 0; i < obs.size(); i++) {
            fl_line(obs[i].first, obs[i].second, obs[(i + 1) % obs.size()].first, obs[(i + 1) % obs.size()].second);
        }
    }

    fl_color(FL_GREEN);
    if (this->operation == 1) {
        fl_circle(p.pos.first, p.pos.second, 2);
    } else if (this->operation == 2) {
        fl_circle(p.pos_in_pair[0].first, p.pos_in_pair[0].second, 2);
        fl_circle(p.pos_in_pair[1].first, p.pos_in_pair[1].second, 2);
    }
}