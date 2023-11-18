#ifndef _MYWINDOW_H_
#define _MYWINDOW_H_

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <vector>
#include <polygon.h>

extern std::vector<std::pair<double, double>> points;
extern bool click1;
extern bool click2;

class mywindow : public Fl_Window {
private:
    Polygon poly;
    double area;
    double scale;   // 假设比例尺是基于100米

public:
    mywindow(int width, int height, const char* title);

    int handle(int event) override;

    void draw() override;

    auto getVc();

    void initPoly();

    void setScale(double scale);

};

void button_callback_1(Fl_Widget* w, void* data);

void button_callback_2(Fl_Widget* w, void* data);

#endif