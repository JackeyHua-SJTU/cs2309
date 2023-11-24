#ifndef _MYWINDOW_H_
#define _MYWINDOW_H_

#include <FL/Fl.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Window.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <vector>
#include <polygon.h>


class mywindow : public Fl_Window {
private:
    Polygon poly;   // vector of points is in poly (poly.points)
    double area;
    double scale;   // 比例尺, pixel per meter
    bool click1;    // 是否点击了第一个按钮
    bool click2;    // 是否点击了第二个按钮
    bool input;    // 是否输入了比例尺

public:
    mywindow(int width, int height, const char* title);

    int handle(int event) override;

    void draw() override;

    auto getVc();

    void pop_back_vc();

    void clear_vc();

    void initPoly();

    void setScale(double scale);

    void click_button_1();

    void click_button_2();

    void finish_input();

};

struct callbackPack {
    Fl_Input* input;
    mywindow* window;

    callbackPack(Fl_Input* input, mywindow* window) : input(input), window(window) {}

};

void button_callback_1(Fl_Widget* w, void* data);

void button_callback_2(Fl_Widget* w, void* data);

void input_callback(Fl_Widget* w, void* data);

#endif