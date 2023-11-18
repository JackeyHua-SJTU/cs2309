#include "superellipse.h"
#include <cmath>    // calculate pow
#include <random>   // randomly select points if required
#include <set>      // store selected points if required

superellipse::superellipse(double a, double b, double m, double n, int N, int width, int height) : Fl_Window(width, height, "superellipse") {
    this->a = a;
    this->b = b;
    this->m = m;
    this->n = n;
    this->N = N;
    this->k = N - 1;
    this->points.resize(N);

    /**
     * @brief 
     * 首先可以看出超椭圆的上下左右端点，根据N的奇偶性，确定选点方式
     *      1. 如果N是奇数，那么中间点取在上或下定点，然后左右对称的取x坐标
     *      2. 如果N是偶数，那么直接左右对称的取x坐标
     */

    if (this->N % 2 == 0) {
        double slot = this->b / (this->N / 2);
        int flag = 1;
        for(int i = 0; i < this->N / 2; ++i) {
            this->points[i] = std::make_pair(-slot * (i + 1), flag * abs(pow(1 - abs(pow(slot * (i + 1) / this->a, this->m)), 1 / this->n) * this->b));
            this->points[i + this->N / 2] = std::make_pair(slot * (i + 1), flag * abs(pow(1 - abs(pow(slot * (i + 1) / this->a, this->m)), 1 / this->n) * this->b));
            // std::cout << "points [" << i << "] is : " << this->points[i].first << " " << this->points[i].second << std::endl;
            flag *= -1;
        }
    } else {
        double slot = this->b / ((this->N - 1) / 2);
        int flag = 1;
        for(int i = 0; i < (this->N - 1) / 2; ++i) {
            this->points[i] = std::make_pair(-slot * (i + 1), flag * abs(pow(1 - abs(pow(slot * (i + 1) / this->a, this->m)), 1 / this->n) * this->b));
            this->points[i + (this->N - 1) / 2 + 1] = std::make_pair(slot * (i + 1), flag * abs(pow(1 - abs(pow(slot * (i + 1) / this->a, this->m)), 1 / this->n) * this->b));
            flag *= -1;
        }
        this->points[(this->N - 1) / 2] = std::make_pair(0, this->b);
    }
}

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
        int flag = 1;
        for(int i = 0; i < this->N / 2; ++i) {
            this->points[i] = std::make_pair(-slot * (i + 1), flag * abs(pow(1 - abs(pow(slot * (i + 1) / this->a, this->m)), 1 / this->n) * this->b));
            this->points[i + this->N / 2] = std::make_pair(slot * (i + 1), flag * abs(pow(1 - abs(pow(slot * (i + 1) / this->a, this->m)), 1 / this->n) * this->b));
            flag *= -1;
        }
    } else {
        double slot = this->b / ((this->N - 1) / 2);
        int flag = 1;
        for(int i = 0; i < (this->N - 1) / 2; ++i) {
            this->points[i] = std::make_pair(-slot * (i + 1), flag * abs(pow(1 - abs(pow(slot * (i + 1) / this->a, this->m)), 1 / this->n) * this->b));
            this->points[i + (this->N - 1) / 2 + 1] = std::make_pair(slot * (i + 1), flag * abs(pow(1 - abs(pow(slot * (i + 1) / this->a, this->m)), 1 / this->n) * this->b));
            flag *= -1;
        }
        this->points[(this->N - 1) / 2] = std::make_pair(0, this->b);
    }
}

void superellipse::valid() {
    if (this->k >= this->N) {
        throw std::runtime_error("k is greater or equal than N. Check k again.");
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
    fl_line(w - 50, h / 2, static_cast<int>(w - 50 - arrowSize * cos(M_PI / 6.0)), static_cast<int>(h / 2 - arrowSize * sin(M_PI / 6.0)));
    fl_line(w - 50, h / 2, static_cast<int>(w - 50 - arrowSize * cos(M_PI / 6.0)), static_cast<int>(h / 2 + arrowSize * sin(M_PI / 6.0)));
    fl_line(w / 2, 50, static_cast<int>(w / 2 - arrowSize * sin(M_PI / 6.0)), static_cast<int>(50 + arrowSize * cos(M_PI / 6.0)));
    fl_line(w / 2, 50, static_cast<int>(w / 2 + arrowSize * sin(M_PI / 6.0)), static_cast<int>(50 + arrowSize * cos(M_PI / 6.0)));


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
        // fltk中y坐标越往下越大，越往上越小
        // 因此为了以正常的坐标系画出图形，需要反过来处理
        fl_circle(x + w / 2, -y + h / 2, 2);
    }
    // if k == N - 1, complete graph
    // else connects to random selected k points

    if (this->k == this->N - 1) {
        for (int i = 0; i < this->points.size(); ++i) {
            for (int j = i + 1; j < this->points.size(); ++j) {
                fl_color(FL_RED);
                int x1 = this->points[i].first + w / 2;
                int y1 = -this->points[i].second + h / 2;
                int x2 = this->points[j].first + w / 2;
                int y2 = -this->points[j].second + h / 2;
                fl_line(x1, y1, x2, y2);
                int arrowLength = 10;
                double angle = atan2(y2 - y1, x2 - x1);
                fl_line(x2 / 3.0 + x1 * 2.0 / 3.0, y2 / 3.0 + y1 * 2.0 / 3.0, static_cast<int>(x2 / 3.0 + x1 * 2.0 / 3.0 - arrowLength * cos(angle - M_PI / 6.0)), static_cast<int>(y2 / 3.0 + y1 * 2.0 / 3.0 - arrowLength * sin(angle - M_PI / 6.0)));
                fl_line(x2 / 3.0 + x1 * 2.0 / 3.0, y2 / 3.0 + y1 * 2.0 / 3.0, static_cast<int>(x2 / 3.0 + x1 * 2.0 / 3.0 - arrowLength * cos(angle + M_PI / 6.0)), static_cast<int>(y2 / 3.0 + y1 * 2.0 / 3.0 - arrowLength * sin(angle + M_PI / 6.0)));
                fl_line(x1 / 3.0 + x2 * 2.0 / 3.0, y1 / 3.0 + y2 * 2.0 / 3.0, static_cast<int>(x1 / 3.0 + x2 * 2.0 / 3.0 + arrowLength * cos(angle - M_PI / 6.0)), static_cast<int>(y1 / 3.0 + y2 * 2.0 / 3.0 + arrowLength * sin(angle - M_PI / 6.0)));
                fl_line(x1 / 3.0 + x2 * 2.0 / 3.0, y1 / 3.0 + y2 * 2.0 / 3.0, static_cast<int>(x1 / 3.0 + x2 * 2.0 / 3.0 + arrowLength * cos(angle + M_PI / 6.0)), static_cast<int>(y1 / 3.0 + y2 * 2.0 / 3.0 + arrowLength * sin(angle + M_PI / 6.0)));
            }
        }
    } else {
        int lowerbound = 0, upperbound = this->N - 1;
        std::random_device rd;  // 随机设备
        std::mt19937 gen(rd());  // 随机数生成器
        std::uniform_int_distribution<int> dist(lowerbound, upperbound);  // 均匀整数分布

        for (int i = 0; i < this->N; ++i) {
            std::set<int> s;
            for (int j = 0; j < this->k; ++j) {
                int rand = dist(gen);
                while (rand == i || s.count(rand)) {
                    rand = dist(gen);
                }
                s.insert(rand);
                fl_color(FL_RED);
                int x1 = this->points[i].first + w / 2;
                int y1 = -this->points[i].second + h / 2;
                int x2 = this->points[rand].first + w / 2;
                int y2 = -this->points[rand].second + h / 2;
                fl_line(x1, y1, x2, y2);
                int arrowLength = 10;
                double angle = atan2(y2 - y1, x2 - x1);
                fl_line(x2 / 3.0 + x1 * 2.0 / 3.0, y2 / 3.0 + y1 * 2.0 / 3.0, static_cast<int>(x2 / 3.0 + x1 * 2.0 / 3.0 - arrowLength * cos(angle - M_PI / 6.0)), static_cast<int>(y2 / 3.0 + y1 * 2.0 / 3.0 - arrowLength * sin(angle - M_PI / 6.0)));
                fl_line(x2 / 3.0 + x1 * 2.0 / 3.0, y2 / 3.0 + y1 * 2.0 / 3.0, static_cast<int>(x2 / 3.0 + x1 * 2.0 / 3.0 - arrowLength * cos(angle + M_PI / 6.0)), static_cast<int>(y2 / 3.0 + y1 * 2.0 / 3.0 - arrowLength * sin(angle + M_PI / 6.0)));
            }
        }
    }

    fl_color(FL_DARK_MAGENTA);
    int x0 = w / 2, y0 = h / 2;

    for (int x = x0 - a; x <= x0 + a; ++x) {
        double y = abs(pow(1 - abs(pow((x - x0) / a, m)), 1 / n) * b);
        fl_point(x, y0 + y);
        fl_point(x, y0 - y);
    }
}