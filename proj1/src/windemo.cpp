#include "windemo.h"
#include <FL/fl_draw.H>
#include <string>
#include <iomanip>
#include <cmath>
#include <sstream>

windemo::windemo(int width, int height, std::vector<std::pair<double, double>> vc, double scale, const char* title) : Fl_Window(width, height, title) {
    this->poly = Polygon(std::move(vc));
    // this->poly.sort();
    this->area = this->poly.area();
    this->scale = scale;
}

void windemo::draw() {
    Fl_Window::draw();

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

    auto f = [](double x) {
        if (x > 0 && x < 1) {
            return -1;
        }
        int a = static_cast<int>(x);
        int cnt = 0;
        while (a) {
            a /= 10;
            ++cnt;
        }
        return cnt;
    };

    std::ostringstream oss;
    int cnt1 = f(this->area);
    int cnt2 = f(this->area / (this->scale * this->scale));
    bool flag1 = (cnt1 == -1);
    bool flag2 = (cnt2 == -1);
    double a1 = this->area;
    if (!flag1) {
        a1 /= pow(10, cnt1 - 1);
    }
    a1 = static_cast<int>(a1 * pow(10, 5 + flag1) + 0.5) / pow(10, 5 + flag1);
    if (flag1) {
        a1 *= 10;
    }
    double a2 = this->area / (this->scale * this->scale);
    if (!flag2) {
        a2 /= pow(10, cnt2 - 1);
    }
    a2 = static_cast<int>(a2 * pow(10, 5 + flag2) + 0.5) / pow(10, 5 + flag2);
    if (flag2) {
        a2 *= 10;
    }
    oss << std::fixed << std::setprecision(5) << a1 << "*10^" << cnt1 - 1; 
    std::string s1 = "pixel area: " + oss.str();
    fl_draw(s1.c_str(), 30, h / 3);

    fl_color(FL_RED);
    std::ostringstream oss2;
    oss2 << std::fixed << std::setprecision(5) << a2 << "*10^" << cnt2 - 1 << " m^2";
    std::string s2 = "real area: " + oss2.str();
    fl_draw(s2.c_str(), 30, h * 2 / 3);
}