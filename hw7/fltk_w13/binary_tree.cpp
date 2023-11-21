#include "binary_tree.h"
#include <FL/Fl.H>
#include <FL/fl_draw.H>

binary_tree::binary_tree(int level, int width, int height) {
    this->level = level;
    this->width = width;
    this->height = height;

    int w = width / (1 << level);
    int h = height / (level + 1);

    add(Point(width / 2, 1));

    for (int i = 1; i <= level; ++i) {
        int s = 1 << i;
        for (int j = 0; j < s; ++j) {
            add(Point(width / 2 - w / 2 * (s - 1) + j * w, i * h + 1));
        }
    }
}

void binary_tree::draw_lines() const {
    int w = width / (1 << level);
    int h = height / (level + 1);
    int x = point(0).x;
    int y = point(0).y;
    int prev = 0;
    for (int i = 0; i < level; ++i) {
        int lb = 1 << i;
        int rb = 1 << (i + 1);
        for (int j = lb; j < rb; ++j) {
            for (int k = prev; k < lb; ++k) {
                fl_line(point(k).x, point(k).y, point(j).x, point(j).y);
            }
        }
        prev = lb;
    }
}



