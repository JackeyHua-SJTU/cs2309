//
// This is example code from Chapter 12.3 "A first example" of
// "Programming -- Principles and Practice Using C++" by Bjarne Stroustrup
//

#include <FL/Fl.H>
#include <FL/Fl_PNG_Image.H>
#include <FL/Fl_JPEG_Image.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <boost/random.hpp>
#include <string>

int rand(int min, int max)
{   
    // unsigned long seed = 10653;
    // static boost::mt19937 rng(seed);
    // static boost::uniform_int<> six(min, max);
    // static boost::variate_generator<boost::mt19937&, boost::uniform_int<> > 
    //     number(rng, six);
    // return number();
    return min + rand() % (max - min);
}


void button_callback(Fl_Widget* w, void* data)
{
    Fl_Window* window = (Fl_Window*)data;
    // std::cout << w->w() << " " << w->h() << std::endl;
    int newX = rand(0, window->w() - w->w());
    int newY = rand(0, window->h() - w->h());
    std::cout << "y range : " << window->h() - w->h() << std::endl;
    std::cout << "current x : " << newX << " current y : " << newY << std::endl;
    w->position(newX, newY);
    window->redraw();
}

//------------------------------------------------------------------------------

int main()
{   
    std::string path;
    std::cout << "Assign the filename of the image.\n";
    std::cin >> path;

    bool flag; // false for png, true for jpeg
    int pos = path.find('.');
    if (path.substr(pos + 1) == "png") {
        flag = false;
    } else if (path.substr(pos + 1) == "jpeg") {
        flag = true;
    } else {
        std::cout << "invalid image format" << std::endl;
        return 1;
    }
    Fl_PNG_Image* png;
    Fl_JPEG_Image* jpeg;
    int w, h;

    if (flag) {
        jpeg = new Fl_JPEG_Image(path.c_str());
        w = jpeg->w();
        h = jpeg->h();
        if (w == 0 || h == 0) {
            std::cout << "can not load the image, check the path." << std::endl;
            return 1;
        }
    } else {
        png = new Fl_PNG_Image(path.c_str());
        w = png->w();
        h = png->h();
        if (w == 0 || h == 0) {
            std::cout << "can not load the image, check the path." << std::endl;
            return 1;
        }
    }

    int width, height;
    std::cout << "Assign the width and height of the window. Notice that width and height should be greater than that of the input figure\n";
    std::cin >> width >> height;

    if (width <= w || height <= h) {
        std::cout << "The width and height should be greater than that of the input figure\n";
        return 1;
    }

    Fl_Window win(width, height, "Random Button");

    Fl_Button button(0, 0, w, h);
    if (flag) {
        button.image(jpeg);
    } else {
        button.image(png);
    }
    button.callback(button_callback, &win);
    win.show();
    return Fl::run();
}

//------------------------------------------------------------------------------
