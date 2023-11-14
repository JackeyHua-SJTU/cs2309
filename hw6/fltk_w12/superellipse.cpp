#include "superellipse.h"
#include "Simple_window.h"
#include "Graph.h"
#include <cmath>    // calculate pow

superellipse::superellipse(double a, double b, double m, double n, int N, int k) {
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


