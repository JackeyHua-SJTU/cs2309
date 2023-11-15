#include "superellipse.h"
#include "Simple_window.h"
#include "Graph.h"
#include <cmath>    // calculate pow

superellipse::superellipse(double a, double b, double m, double n, int N, int k, int width, int height) : Fl_Window(width, height, "superellipse") {
    this->a = a;
    this->b = b;
    this->m = m;
    this->n = n;
    this->N = N;
    this->k = k;
    this->points.resize(N);

    /**
     * @brief 
     * 首先可以看出超椭圆的上下左右端点，根据N的奇偶性，确定选点方式
     *      1. 如果N是奇数，那么中间点取在上或下定点，然后左右对称的取x坐标
     *      2. 如果N是偶数，那么直接左右对称的取x坐标
     */

    if (this->N % 2 == 0) {
        double slot = this->b / (this->N / 2);
        for(int i = 0; i < this->N / 2; ++i) {
            this->points[i] = std::make_pair(-slot * (i + 1), abs(pow(1 - pow(slot * (i + 1) / this->a, this->m), 1 / this->n)) * this->b);
            this->points[i + this->N / 2] = std::make_pair(slot * (i + 1), abs(pow(1 - pow(slot * (i + 1) / this->a, this->m), 1 / this->n)) * this->b);
        }
    } else {
        double slot = this->b / ((this->N - 1) / 2);
        for(int i = 0; i < (this->N - 1) / 2; ++i) {
            this->points[i] = std::make_pair(-slot * (i + 1), abs(pow(1 - pow(slot * (i + 1) / this->a, this->m), 1 / this->n)) * this->b);
            this->points[i + (this->N - 1) / 2 + 1] = std::make_pair(slot * (i + 1), abs(pow(1 - pow(slot * (i + 1) / this->a, this->m), 1 / this->n)) * this->b);
        }
        this->points[(this->N - 1) / 2] = std::make_pair(0, this->b);
    }
}

void superellipse::draw() {
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