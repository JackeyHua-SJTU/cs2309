#include "binary_tree.h"
#include <FL/Fl.H>
#include <FL/fl_draw.H>

binary_tree::binary_tree(int level, int width, int height) {
    this->level = level;
    this->width = width;
    this->height = height;

    int w = width / ((1 << level) + 1);
    int h = height / (level + 1);

    add(Point(width / 2, 10));

    for (int i = 1; i <= level; ++i) {
        int s = 1 << i;
        for (int j = 0; j < s; ++j) {
            add(Point(width / 2 - w / 2 * (s - 1) + j * w, i * h + 1));
        }
    }
}

void binary_tree::draw_lines() const {
    int num = (level == 0) ? 1 : (1 << level) - 1;
    
    for (int i = 0; i < (1 << (level + 1)) - 1; ++i) {
        fl_circle(point(i).x, point(i).y, 2);
    }

    for (int i = 0; i < num; ++i) {
        fl_color(FL_RED);
        fl_line(point(i).x, point(i).y, point(2 * i + 1).x, point(2 * i + 1).y);
        fl_line(point(i).x, point(i).y, point(2 * i + 2).x, point(2 * i + 2).y);
    }
    
}



