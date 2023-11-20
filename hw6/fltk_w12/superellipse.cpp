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
    if (this->m <= 0) {
        throw std::runtime_error("m is less or equal than 0. Check m again.");
    }
    if (this->n <= 0) {
        throw std::runtime_error("n is less or equal than 0. Check n again.");
    }
    if (this->N <= 0) {
        throw std::runtime_error("N is less or equal than 0. Check N again.");
    }
    if (this->k <= 0) {
        throw std::runtime_error("k is less or equal than 0. Check k again.");
    }
    if (this->k >= this->N) {
        throw std::runtime_error("k is greater or equal than N. Check k again.");
    }
}

void superellipse::draw() {
    Fl_Window::draw();

    // get window width and height
    int w = this->w();
    int h = this->h();

    // draw x and y axis at the center of the window
    fl_color(FL_BLACK);
    fl_line(30, h / 2, w - 30, h / 2);  // x轴
    fl_line(w / 2, 30, w / 2, h - 30);  // y轴

    // draw arrow on the end of x and y axis
    int arrowSize = 10;
    fl_line(w - 30, h / 2, static_cast<int>(w - 30 - arrowSize * cos(M_PI / 6.0)), static_cast<int>(h / 2 - arrowSize * sin(M_PI / 6.0)));
    fl_line(w - 30, h / 2, static_cast<int>(w - 30 - arrowSize * cos(M_PI / 6.0)), static_cast<int>(h / 2 + arrowSize * sin(M_PI / 6.0)));
    fl_line(w / 2, 30, static_cast<int>(w / 2 - arrowSize * sin(M_PI / 6.0)), static_cast<int>(30 + arrowSize * cos(M_PI / 6.0)));
    fl_line(w / 2, 30, static_cast<int>(w / 2 + arrowSize * sin(M_PI / 6.0)), static_cast<int>(30 + arrowSize * cos(M_PI / 6.0)));


    // add "X Axis" and "Y Axis" to the end of x and y axis
    fl_color(FL_BLACK);
    fl_font(FL_HELVETICA_BOLD, 14);
    fl_draw("X Axis", w - 50, h / 2 + 40);
    fl_draw("Y Axis", w / 2 + 10, 30);

    // list points on x axis
    for (int i = 1; i <= w / 100 - 1; i++) {
        int x_pos = w / 2 + i * 50;
        int x_neg = w / 2 - i * 50;
        int y = h / 2;
        fl_circle(x_pos, y, 2);
        fl_circle(x_neg, y, 2);
        fl_draw(std::to_string(i * 50).c_str(), x_pos - 5, y + 15);
        fl_draw(std::to_string(i * 50).c_str(), x_neg - 5, y - 15);
    }
        
    // list points on y axis
    for (int i = 1; i <= h / 100 - 1; i++) {
        int x = w / 2;
        int y_pos = h / 2 - i * 50;
        int y_neg = h / 2 + i * 50;
        fl_circle(x, y_pos, 2);
        fl_circle(x, y_neg, 2);
        fl_draw(std::to_string(i * 50).c_str(), x - 25, y_pos + 5);
        fl_draw(std::to_string(i * 50).c_str(), x - 25, y_neg + 5);
    }

    // draw all points in red
    for (auto&& [x, y] : this->points) {
        fl_color(FL_RED);
        // fltk中y坐标越往下越大，越往上越小
        // 因此为了以正常的坐标系画出图形，需要反过来处理
        fl_circle(x + w / 2, -y + h / 2, 2);
    }

    // if k == N - 1, complete graph
    // else connects to random selected k points
    // ! All Lines here are directed
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
        // random select k different indexs in [0, N - 1] except itself
        int lowerbound = 0, upperbound = this->N - 1;
        std::random_device rd;  
        std::mt19937 gen(rd());  
        std::uniform_int_distribution<int> dist(lowerbound, upperbound);  // uniform distribution in [lowerbound, upperbound]

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
    // draw the superellipse
    fl_color(FL_DARK_MAGENTA);
    int x0 = w / 2, y0 = h / 2;

    double x_prev = x0 - a;
    double y_prev = abs(pow(1 - abs(pow((x_prev - x0) / a, m)), 1 / n) * b);
    fl_point(x_prev, y0 + y_prev);
    fl_point(x_prev, y0 - y_prev);

    for (int x = x0 - a + 1; x <= x0 + a; ++x) {
        double y = abs(pow(1 - abs(pow((x - x0) / a, m)), 1 / n) * b);
        fl_point(x, y0 + y);
        fl_point(x, y0 - y);
        fl_line(x_prev, y0 + y_prev, x, y0 + y);
        fl_line(x_prev, y0 - y_prev, x, y0 - y);
        x_prev = x;
        y_prev = y;
    }
}