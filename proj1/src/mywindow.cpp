#include "mywindow.h"
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <FL/Fl_Input.H>


mywindow::mywindow(int width, int height, const char *title) : Fl_Window(width, height, title) {
    resizable(this);
    this->area = 0.0;
    this->scale = 0.0;
    this->click1 = false;
    this->click2 = false;
    this->input = false;
}

void mywindow::click_button_1() {
    this->click1 = true;
    return ;
}

void mywindow::click_button_2() {
    this->click2 = true;
    return ;
}

int mywindow::handle(int event)  {
    if (!this->click2 && event == FL_PUSH) {
        int x = Fl::event_x();  // x axis clicked by mouse
        int y = Fl::event_y();  // y axis clicked by mouse
        if (this->input) {
            this->poly.points.emplace_back(x, y);  // store it into vector
        }
        std::cout << "current size is " << this->poly.points.size() << std::endl;
        // print coordinates to console
        printf("点击坐标：(%d, %d)\n", x, y);
        this->redraw();  // 重绘窗口
    }
    return Fl_Window::handle(event);
}

void mywindow::draw() {
    Fl_Window::draw();

    // 获取窗口的宽度和高度
    int w = this->w();
    int h = this->h();

    // 绘制坐标轴
    fl_color(FL_BLACK);
    // (300, h - 50)为坐标原点
    fl_line(300, h - 50, w - 50, h - 50);  // x轴
    fl_line(300, 50, 300, h - 50);  // y轴

    // 绘制轴箭头
    int arrowSize = 10;
    fl_line(w - 50, h - 50, static_cast<int>(w - 50 - arrowSize * cos(M_PI / 6.0)), static_cast<int>(h - 50 - arrowSize * sin(M_PI / 6.0)));
    fl_line(w - 50, h - 50, static_cast<int>(w - 50 - arrowSize * cos(M_PI / 6.0)), static_cast<int>(h - 50 + arrowSize * sin(M_PI / 6.0)));
    fl_line(300, 50, static_cast<int>(300 - arrowSize * sin(M_PI / 6.0)), static_cast<int>(50 + arrowSize * cos(M_PI / 6.0)));
    fl_line(300, 50, static_cast<int>(300 + arrowSize * sin(M_PI / 6.0)), static_cast<int>(50 + arrowSize * cos(M_PI / 6.0)));

    // 绘制轴名称
    fl_color(FL_BLACK);
    fl_font(FL_HELVETICA_BOLD, 14);
    fl_draw("X Axis", w - 50, h - 30);
    fl_draw("Y Axis", 250, 50);

    // 在x轴上标注点
    for (int i = 1; i <= w / 50 - 8; i++) {
        int x_pos = 300 + i * 50;
        int y = h - 50;
        fl_circle(x_pos, y, 2);
        fl_draw(std::to_string(i * 50).c_str(), x_pos - 5, y + 15);
    }

    // 在y轴上标注点
    for (int i = 1; i <= h / 50 - 3; i++) {
        int x = 300;
        int y_pos = h - 50 - i * 50;
        fl_circle(x, y_pos, 2);
        //fl_circle(x, y_neg, 2);
        fl_draw(std::to_string(i * 50).c_str(), x - 30, y_pos + 5);
        // fl_draw(std::to_string(i * 50).c_str(), x - 25, y_neg + 5);
    }

    // 绘制所有点击坐标
    for (auto&& point : this->poly.points) {
        fl_color(FL_RED);  // 设置绘制颜色
        fl_circle(point.first, point.second, 2);  // 绘制点
    }

    if (click1) {
        fl_color(FL_RED);
        std::string s1 = "scale: " + std::to_string(this->scale) + " pixels/meter";
        fl_draw(s1.c_str(), 30, h * 3 / 5);
    }

    if (click2) {
        fl_color(FL_RED);
        int size = this->poly.points.size();
        auto points = this->poly.points;
        for (int i = 0; i < size; ++i) {
            fl_line(points[i].first, points[i].second, points[(i + 1) % size].first, points[(i + 1) % size].second);
        }
        fl_color(FL_MAGENTA);
        fl_begin_polygon();
        for (auto&& point : points) {
            fl_vertex(point.first, point.second);
        }
        fl_end_polygon();

        fl_color(FL_RED);
        std::ostringstream oss;
        // rounding and scientific notation
        oss << std::scientific << std::setprecision(6) << this->area;
        std::string s1 = "pixel area: " + oss.str();
        fl_draw(s1.c_str(), 30, h * 4 / 5);

        fl_color(FL_RED);
        std::ostringstream oss2;
        oss2 << std::scientific << std::setprecision(6) << this->area * this->scale * this->scale;
        std::string s2 = "real area: " + oss2.str();
        fl_draw(s2.c_str(), 30, h * 6 / 7);
    }
}

void mywindow::initPoly() {
    poly.sort();
    auto size = poly.area();
    this->area = size;
    std::cout << size << std::endl;
}

auto mywindow::getVc() {
    return poly.points;
}

void mywindow::setScale(double scale) {
    this->scale = scale;
}

void mywindow::pop_back_vc() {
    this->poly.points.pop_back();
}

void mywindow::clear_vc() {
    this->poly.points.clear();
}

void mywindow::finish_input() {
    this->input = true;
    return ;
}

void button_callback_1(Fl_Widget* w, void* data) {
    // std::cout << "button_callback_1 " << this->poly.points.size() <<  std::endl;  // 输出调试信息
    auto win = (mywindow*)data;
//    fl_line(points[0].first, points[0].second, points[1].first, points[1].second);
//    win->redraw();
    win->click_button_1();
    auto points = win->getVc();
    auto p1 = points[0];
    auto p2 = points[1];
    win->setScale(sqrt(pow(p1.first - p2.first, 2) + pow(p1.second - p2.second, 2)) / 100);
    win->clear_vc();
    std::cout << "button_callback_1 " << points.size() <<  std::endl;
    w->hide();
    win->redraw();
}

void button_callback_2(Fl_Widget* w, void* data) {
    std::cout << "button_callback_2" << std::endl;  // 输出调试信息
    auto win = (mywindow*)data;
    // std::cout << "button_callback_2 " << points.size() <<  std::endl;
    win->pop_back_vc();
    win->initPoly();
    // std::cout << "button_callback_2 " << points.size() <<  std::endl;
    auto points = win->getVc();
    int size = points.size();
    std::cout << "current size is " << size << std::endl;
    win->click_button_2();
    // std::string s = "area is " + std::to_string(ans);

    // auto box = new Fl_Box(30, 300, 250, 50, s.c_str());

    win->redraw();
}

// todo: parse input

void input_callback(Fl_Widget* widget, void* data) {
    auto pack = static_cast<callbackPack*>(data);
    auto input = pack->input;
    auto win = pack->window;
    const char* value = input->value();
    win->finish_input();
    input->hide();
    widget->hide();
    std::cout << "User input: " << value << std::endl;
}