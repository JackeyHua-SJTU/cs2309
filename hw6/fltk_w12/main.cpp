//
// This is example code from Chapter 12.3 "A first example" of
// "Programming -- Principles and Practice Using C++" by Bjarne Stroustrup
//

#include "superellipse.h"
#include "poly.h"
#include <FL/Fl_Box.H>
#include <FL/Fl_Window.H>
#include <FL/fl_draw.H>
#include <string>
#include <iostream>

using std::vector;
using std::pair;
using std::cout;
using std::endl;

//------------------------------------------------------------------------------

// 继承Fl_Window创建自定义窗口类
class MyWindow : public Fl_Window {
private:
    std::vector<std::pair<double, double>> points;

public:
    MyWindow(int width, int height, std::vector<std::pair<double, double>> points) : Fl_Window(width, height) {
        // 设置窗口标题
        this->points = points;
        label("坐标轴示例");
    }

    void draw() override {
        Fl_Window::draw();

        // 获取窗口的宽度和高度
        int w = this->w();
        int h = this->h();

        // 绘制坐标轴
        fl_color(FL_BLACK);
        fl_line(50, h / 2, w - 50, h / 2);  // x轴
        fl_line(w / 2, 50, w / 2, h - 50);  // y轴

        // 绘制轴箭头
        int arrowSize = 10;
        fl_polygon(w - 50, h / 2, w - 50 - arrowSize, h / 2 - arrowSize, w - 50 - arrowSize, h / 2 + arrowSize);  // x轴箭头
        fl_polygon(w / 2, 50, w / 2 - arrowSize, 50 + arrowSize, w / 2 + arrowSize, 50 + arrowSize);  // y轴箭头

        // 绘制轴名称
        fl_color(FL_BLACK);
        fl_font(FL_HELVETICA_BOLD, 14);
        fl_draw("X Axis", w - 50, h / 2 + 20);
        fl_draw("Y Axis", w / 2 + 10, 50);

        // 在x轴上标注点
        for (int i = 1; i <= 6; i++) {
            int x_pos = w / 2 + i * 50;
            int x_neg = w / 2 - i * 50;
            int y = h / 2;
            fl_circle(x_pos, y, 2);
            fl_circle(x_neg, y, 2);
            fl_draw(std::to_string(i * 50).c_str(), x_pos - 5, y + 15);
            fl_draw(std::to_string(i * 50).c_str(), x_neg - 5, y - 15);
        }
        
        // 在y轴上标注点
        for (int i = 1; i <= 4; i++) {
            int x = w / 2;
            int y_pos = h / 2 - i * 50;
            int y_neg = h / 2 + i * 50;
            fl_circle(x, y_pos, 2);
            fl_circle(x, y_neg, 2);
            fl_draw(std::to_string(i * 50).c_str(), x - 25, y_pos + 5);
            fl_draw(std::to_string(i * 50).c_str(), x - 25, y_neg + 5);
        }

        for (auto&& [x, y] : this->points) {
            fl_color(FL_RED);
            fl_circle(x + w / 2, y + h / 2, 2);
        }

        for (int i = 0; i < this->points.size(); ++i) {
            for (int j = i; j < this->points.size(); ++j) {
                fl_color(FL_RED);
                fl_line(this->points[i].first + w / 2, this->points[i].second + h / 2, this->points[j].first + w / 2, this->points[j].second + h / 2);
            }
        }

    }
};

int main() {

    vector<pair<double, double>> points_1= {
        {0, 0},
        {100, 0},
        {100, 100},
        {0, 100}
    };

    vector<pair<double, double>> points_2 = {{0, 0}, {100, 0}, {100, 100}, {0, -100}};

    vector<pair<double, double>> points_3 = {{0, 0}, {0, 1}, {0, 2}};

    vector<pair<double, double>> points_4= {
    {116.403322, 39.920255},
    {116.385726, 39.909893},
    {116.410703, 39.897555},
    {116.402292, 39.892353},
    {116.389846, 39.891365}
    };
    vector<pair<double, double>> points_5= {
    {116.403322, 39.920255},
    {116.410703, 39.897555},
    {116.402292, 39.892353},
    {116.389846, 39.891365}
    };


    poly p1(points_1);
    poly p2(points_2);
    poly p3(points_3);
    poly p4(points_4);
    poly p5(points_5);

    cout << "p1 is " << (p1.isValid() ? "valid" : "invalid") << endl;
    cout << "p2 is " << (p2.isValid() ? "valid" : "invalid") << endl;
    cout << "p3 is " << (p3.isValid() ? "valid" : "invalid") << endl;
    cout << "p4 is " << (p4.isValid() ? "valid" : "invalid") << endl;
    cout << "p5 is " << (p5.isValid() ? "valid" : "invalid") << endl;

    return 0;
    



    // superellipse s(300, 200, 2, 2, 10, 10);
    
    // // 创建窗口对象
    // MyWindow window(800, 600, s.getPoints());

    // // 显示窗口
    // window.show();

    // // 运行FLTK事件循环
    // return Fl::run();
}

//------------------------------------------------------------------------------
