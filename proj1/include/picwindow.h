#ifndef _PICWINDOW_H_
#define _PICWINDOW_H_

#include <FL/Fl.H>
#include <GL/gl.h>
#include <FL/Fl_Gl_Window.H>
#include <FL/Fl_Button.H>
#include "polygon.h"

class picwindow : public Fl_Gl_Window {
private:
    Polygon poly;   // vector of points is in poly (poly.points)
    double area;
    double scale;   // 假设比例尺是基于100米
    bool click1;    // 是否点击了第一个按钮
    bool click2;    // 是否点击了第二个按钮
    Fl_Button* button1;
    Fl_Button* button2;

    static void button1_callback(Fl_Widget *w, void *data);

    static void button2_callback(Fl_Widget *w, void *data);

public:
    picwindow(int x, int y, int w, int h, const char* l = 0);

    void draw() override;

    int handle(int event) override;

};

#endif

