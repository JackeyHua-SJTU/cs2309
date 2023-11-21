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

//------------------------------------------------------------------------------

int main()
{
    Simple_window win1(Point(100,100),800,600,"clock");
    binary_tree b(5, 800, 600);
    win1.attach(b);

    star s(10, 800, 600);
    s.show();
    win1.redraw();

    return Fl::run();

}

//------------------------------------------------------------------------------
