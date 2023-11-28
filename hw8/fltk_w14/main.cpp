//
// This is example code from Chapter 12.3 "A first example" of
// "Programming -- Principles and Practice Using C++" by Bjarne Stroustrup
//

#include <FL/Fl.H>
#include <FL/Fl_PNG_Image.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <boost/random.hpp>
#include <string>

int rand(int min, int max)
{   
    unsigned long seed = 10653;
    static boost::mt19937 rng(seed);
    static boost::uniform_int<> six(min, max);
    static boost::variate_generator<boost::mt19937&, boost::uniform_int<> > 
        number(rng, six);
    return number();
}


void button_callback(Fl_Widget* w, void* data)
{
    Fl_Window* window = (Fl_Window*)data;
    // std::cout << w->w() << " " << w->h() << std::endl;
    int newX = rand(0, window->w() - w->w());
    int newY = rand(0, window->h() - w->h());
    w->position(newX, newY);
    window->redraw();
}

//------------------------------------------------------------------------------

int main()
{
    int width, height;
    std::cout << "Assign the width and height of the window.\n";
    std::cin >> width >> height;
    Fl_Window win(width, height, "Random Button");

    std::string filename;
    std::cout << "Assign the filename of the image.\n";
    std::cin >> filename;
    Fl_PNG_Image img(filename.c_str());
    int w = img.w();
    int h = img.h();
    Fl_Button button(0, 0, w, h);
    button.image(img);
    button.callback(button_callback, &win);
    win.show();
    return Fl::run();
}

//------------------------------------------------------------------------------
