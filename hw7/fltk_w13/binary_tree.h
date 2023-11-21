#ifndef _BINARY_TREE_H_
#define _BINARY_TREE_H_

#include "Graph.h"

class binary_tree : public Graph_lib::Shape {
    private:
        int level;
        int width;
        int height;

    public:
        binary_tree(int level, int width, int height);
        void draw_lines() const;
};

#endif