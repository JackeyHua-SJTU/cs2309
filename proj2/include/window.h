#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <vector>
#include "poly.h"

class window : public Fl_Window {
    private:
        std::vector<std::pair<double, double>> points;
        int operation;  // camera number
        

    public:
        window(int w, int h, std::vector<std::pair<double, double>> points, int op, const char *title);
        window(int w, int h, std::vector<std::pair<double, double>> points, std::vector<std::pair<double, double>> obs, int op, const char *title);
        window(int w, int h, const char *title) : Fl_Window(w, h, title) { }
        void draw() override;
        poly p;
};

#endif