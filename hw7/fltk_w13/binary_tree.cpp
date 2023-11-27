#include "binary_tree.h"
#include <FL/Fl.H>
#include <FL/fl_draw.H>

binary_tree::binary_tree(int level, int width, int height) {
    this->level = level;
    this->width = width;
    this->height = height;

    double w = width / ((1 << (level - 1)) + 2.0);
    double h = height / (level + 0.3);

    // add(Point(width / 2, h / 5));

    for (int i = 1; i <= level; ++i) {
        int s = 1 << (i - 1);
        for (int j = 0; j < s; ++j) {
            add(Point(width / 2.0 - w / 2.0 * (s - 1) + j * w, (i - 1.0) * h + h / 4.0));
        }
    }
}

void binary_tree::draw_lines() const {
    if (this->level == 0) {
        return;
    }
    
    for (int i = 0; i < (1 << level) - 1; ++i) {
        fl_circle(point(i).x, point(i).y, 2);
    }

    for (int i = 0; i < (1 << (level - 1)) - 1; ++i) {
        fl_color(FL_RED);
        fl_line(point(i).x, point(i).y, point(2 * i + 1).x, point(2 * i + 1).y);
        fl_line(point(i).x, point(i).y, point(2 * i + 2).x, point(2 * i + 2).y);
    }
}



