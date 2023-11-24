#ifndef _WINDEMO_H_
#define _WINDEMO_H_

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include "polygon.h"
#include <vector>

class windemo : public Fl_Window {
    private:
        Polygon poly;
        double scale;
        double area;

    public:
        windemo(int width, int height, std::vector<std::pair<double, double>> vc, double scale, const char* title);

        void draw() override;
};

#endif