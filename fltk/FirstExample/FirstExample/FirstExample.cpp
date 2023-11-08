#include "Simple_window.h" 	// stuff to deal with your system��s windows
#include "Graph.h" 		// graphical shapes
#include <unistd.h>

int main()
{

    Simple_window win(Point(100,100),400,400,"clock");
    Graph_lib::Circle cloc(Point(200,200),180);
        Graph_lib::Circle cloc2(Point(200,200),150);
        Graph_lib::Circle cloc3(Point(200,200),100);
        win.attach(cloc);

        sleep(50);
        cloc.move(100,100);
        //win.attach(cloc2) ;
        win.redraw();

win.wait_for_button();
//win.wait_for_button();
/*	using namespace Graph_lib;	// use our graphics interface library

	Point tl(100,200);			// a point (obviously)

	Simple_window win(tl,600,400,"Canvas"); 	// make a simple window

	Graph_lib::Polygon poly;		// make a shape (a polygon, obviously)

	poly.add(Point(300,200));	// add three points to the polygon
	poly.add(Point(350,100));
	poly.add(Point(400,200));

	poly.set_color(Color::red);	    // make the polygon red (obviously)
    Graph_lib::Text t(Point(200,200),"debug");
    //t.set_color(Color::black);
    //t.draw();
    win.color();
    win.color
    win.attach(t);
	win.attach(poly);		// connect poly to the window
    win.detach(poly);
    win.hide();
    win.redraw();

//	Circle c(Point(200,200),50);
//	c.draw();
//	win.redraw();
//	win.attach(c);
	win.wait_for_button();	// give control to the display engine
	*/
}


/*

//
// This is example code from Chapter 16.5 "An example" of
// "Programming -- Principles and Practice Using C++" by Bjarne Stroustrup
//

#include <string.h>
#include <iostream>
#include <sstream>
#include "Graph.h"        // get access to our graphics library facilities
#include "GUI.h"
#include "Window.h"

using namespace Graph_lib;
using namespace std;

//------------------------------------------------------------------------------

struct Lines_window : Graph_lib::Window {
    Lines_window(Point xy, int w, int h, const string& title );
    Open_polyline lines;
private:
    Button next_button;        // add (next_x,next_y) to lines
    Button quit_button;
    In_box next_x;
    In_box next_y;
    Out_box xy_out;

    static void cb_next(Address, Address); // callback for next_button
    void next();
    static void cb_quit(Address, Address); // callback for quit_button
    void quit();
};

//------------------------------------------------------------------------------

Lines_window::Lines_window(Point xy, int w, int h, const string& title)
    :Graph_lib::Window(xy,w,h,title),
    next_button(Point(x_max()-150,0), 70, 20, "Next point", cb_next),
    quit_button(Point(x_max()-70,0), 70, 20, "Quit", cb_quit),
    next_x(Point(x_max()-310,0), 50, 20, "next x:"),
    next_y(Point(x_max()-210,0), 50, 20, "next y:"),
    xy_out(Point(100,0), 100, 20, "current (x,y):")
{
    attach(next_button);
    attach(quit_button);
    attach(next_x);
    attach(next_y);
    attach(xy_out);
    attach(lines);
}

//------------------------------------------------------------------------------

void Lines_window::cb_quit(Address, Address pw)    // "the usual"
{
       reference_to<Lines_window>(pw).quit();
}

//------------------------------------------------------------------------------

void Lines_window::quit()
{
    hide();        // curious FLTK idiom for delete window
}

//------------------------------------------------------------------------------

void Lines_window::cb_next(Address, Address pw)     // "the usual"
{
    reference_to<Lines_window>(pw).next();
}

//------------------------------------------------------------------------------

void Lines_window::next()
{
    int x = next_x.get_int();

    string y1="123";
    next_y.get_string();
    int y = atoi(y1.c_str());//next_y.get_int();

    lines.add(Point(x,y));

    // update current position readout:
    stringstream ss;
    ss << '(' << x << ',' << y << ')';
    xy_out.put(ss.str());

      redraw();
}

//------------------------------------------------------------------------------

int main()
try {
    Lines_window win(Point(100,100),600,400,"lines");
    return gui_main();
}
catch(exception& e) {
    cerr << "exception: " << e.what() << '\n';
    return 1;
}
catch (...) {
    cerr << "Some exception\n";
    return 2;
}

//------------------------------------------------------------------------------

*/
//------------------------------------------------------------------------------

//
// This is example code from Chapter 12.3 "A first example" of
// "Programming -- Principles and Practice Using C++" by Bjarne Stroustrup
//

//#include "Simple_window.h"    // get access to our window library
//#include "Graph.h"
//#include "std_lib_facilities.h"
//#include <iostream>
//using namespace std;           // get access to our graphics library facilities

//------------------------------------------------------------------------------

/*int main()
try
{
    using namespace Graph_lib;   // our graphics facilities are in Graph_lib
*/

    //fl_line(300, 300, 500, 600);
/*

    Simple_window win(Point(100,100),600,400,"16*16 color matrix");
    Vector_ref<Graph_lib::Rectangle> vr;

    for (int i = 0; i<16; ++i)
        for (int j = 0; j<16; ++j) {
            vr.push_back(new Graph_lib::Rectangle(Point(i*20,j*20),20,20));
            vr[vr.size()-1].set_fill_color(i*16+j);
            win.attach(vr[vr.size()-1]);
        }

        win.redraw();
    win.wait_for_button();       // Display!
*/
    /*
    Point pt(100,100);
    Simple_window win20(pt,600,400,"16*16 color matrix");
    Vector_ref<Rectangle> vr;  // use like vector
    // but imagine that it holds references to objects
    for (int i = 0; i<16; ++i)  		// i is the horizontal coordinate
    {
        for (int j = 0; j<16; ++j)  	// j is the vertical coordinate
        {
            vr.push_back(new Rectangle(Point(i*20,j*20),20,20));
            vr[vr.size()-1].set_fill_color(i*16+j);
            win20.attach(vr[vr.size()-1]);
        }
    }
    */

/*
        Simple_window win(Point(100,100),600,400,"Canvas");     // make a window

        Line horizontal(Point(100,100),Point(200,100));     	// make a horizontal line
        Line vertical(Point(150,50),Point(150,150));	// make a vertical line


        // horizontal.set_color(Color::black);
        // vertical.set_color(Color::black);

        win.attach(horizontal);	// attach the lines to the window
        win.attach(vertical);


        Image ii(Point(100,50),"1.jpg");
        win.attach(ii);
win.redraw();
       win.wait_for_button();
*/

        /*
            Point tl(100,100);           // to become top left  corner of window

            Simple_window win(tl,600,400,"Canvas");    // make a simple window
        */
        /*
              Graph_lib::Polygon poly;                // make a shape (a polygon)

                poly.add(Point(300,200));    // add a point
                poly.add(Point(350,100));    // add another point
                poly.add(Point(400,200));    // add a third point
               poly.set_color(Color::red);  // adjust properties of poly

                win.attach (poly);           // connect poly to the window

              // win.set_label("Canvas #6");

            Graph_lib::Closed_polyline poly_rect;
            poly_rect.add(Point(100,50));
            poly_rect.add(Point(200,50));
            poly_rect.add(Point(200,100));
            poly_rect.add(Point(100,100));

            poly_rect.set_color(Color::red);
            win.attach (poly_rect);           // connect poly to the window




            win.redraw();
            */
   //     win20.wait_for_button();       // give control to the display engine
/*    }
    catch(exception& e)
    {
        cerr << "exception: " << e.what() << '\n';
        return 1;
    }
    catch (...)
    {
        cerr << "Some exception\n";
        return 2;
    }
*/
//------------------------------------------------------------------------------
