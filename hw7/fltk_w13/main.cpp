//
// This is example code from Chapter 12.3 "A first example" of
// "Programming -- Principles and Practice Using C++" by Bjarne Stroustrup
//

// #include "Simple_window.h"    // get access to our window library
// #include "Graph.h"            // get access to our graphics library facilities
#include <star.h>
#include <binary_tree.h>
#include <FL/Fl.H>
#include "Simple_window.h"
#include <iostream>

//------------------------------------------------------------------------------

int main()
{
//-----------------------------------TASK1--------------------------------------
    // // 截图所用的测试案例
    // star s(10, 800, 600);
    // s.show();
    int num, width1, height1;
    std::cout << "Please input the number of vertices and the width and height of the window.\n";
    std::cin >> num >> width1 >> height1;
    star s(num, width1, height1, "star");
    s.show();

//-----------------------------------TASK2--------------------------------------
    // // 截图所用的测试案例
    // Simple_window win1(Point(100,100),800,600,"binary_tree");
    // binary_tree b(5, 800, 600);
    // win1.attach(b);
    int level, width2, height2;
    std::cout << "Please input the level of the binary tree and the width and height of the window.\n";
    std::cin >> level >> width2 >> height2;
    Simple_window win1(Point(100,100), width2, height2, "binary_tree");
    binary_tree b(level, width2, height2);
    win1.attach(b);

    return Fl::run();
}

//------------------------------------------------------------------------------
