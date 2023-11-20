#include "superellipse.h"
#include "poly.h"
#include <FL/Fl.H>
#include <iostream>
#include <regex>

using std::vector;
using std::pair;
using std::cout;
using std::endl;
using std::cin;
using std::getline;

//------------------------------------------------------------------------------


// ! 在运行本文件前，请先看当前文件夹下的README.md
// @author jackeyhua


int main() {
//----------------------------------TASK1----------------------------------------
    // * 此部分为截图中用的测试数据，如需验证，只需在命令行输入对应的数据即可

    // superellipse s1(300, 200, 2, 2, 10, 11, 800, 600);    // 指定k为11，需要抛出异常
    // superellipse s2(300, 200, 2, 2, 10, 800, 600);    // 指定k为9
    // superellipse s3(300, 200, 3, 3, 10, 5, 800, 600);   // 指定k为5
    // try {
    //     s1.valid();
    //     s1.show();
    // }
    // catch (const std::exception& e) {
    //     std::cout << "Exception caught : " << e.what() << std::endl;
    // }

    // try {
    //     s2.valid();
    //     s2.show();
    // }
    // catch (const std::exception& e) {
    //     std::cout << "Exception caught : " << e.what() << std::endl;
    // }

    // try {
    //     s3.valid();
    //     s3.show();
    // }
    // catch (const std::exception& e) {
    //     std::cout << "Exception caught : " << e.what() << std::endl;
    // }

    std::string input;
    getline(cin, input);
    std::regex pattern_float(R"(\-?[0-9]+(\.[0-9]+)?)");    // a,b,m,n 为浮点数, N, K 为整数, 可以识别整数

    double a, b, m, n;
    int N, K = -1, width = 800, height = 600;

    std::sregex_iterator it(input.begin(), input.end(), pattern_float);
    std::sregex_iterator end;
    int rnd = 0;
    while (it != end) {
        if (rnd == 0) {
            a = std::stod(it->str());
        } else if (rnd == 1) {
            b = std::stod(it->str());
        } else if (rnd == 2) {
            m = std::stod(it->str());
        } else if (rnd == 3) {
            n = std::stod(it->str());
        } else if (rnd == 4) {
            N = std::stoi(it->str());
        } else if (rnd == 5) {
            K = std::stoi(it->str());
        } else if (rnd == 6) {
            width = std::stoi(it->str());
        } else if (rnd == 7) {
            height = std::stoi(it->str());
        }
        ++it;
        ++rnd;
    }

    if (K == -1) {
        K = N - 1;
    }
    superellipse s(a, b, m, n, N, K, width, height);
    try {
        s.valid();
        s.show();
    }
    catch (const std::exception& e) {
        std::cout << "Exception caught : " << e.what() << std::endl;
    }
    

//---------------------------------TASK2-----------------------------------------
    // * 此部分为截图中用的测试数据，如需验证，只需在命令行输入对应的数据即可

    // vector<pair<double, double>> valid_points = {{0, 0}, {100, 0}, {100, 100}, {-100, 0}, {-200, -200}, {-100, -100}};

    // vector<pair<double, double>> invalid_points = {{0, 0}, {100, 0}, {100, 100}, {0, -100}};

    // poly p1(valid_points, 800, 600);
    // poly p2(invalid_points, 800, 600);

    // if (p1.isValid()) {
    //     p1.show();
    // } else {
    //     cout << "poly p1 is not valid\n"; 
    // }

    // if (p2.isValid()) {
    //     p2.show();
    // } else {
    //     cout << "poly p2 is not valid\n";
    // }

    vector<pair<double, double>> vc;
    int width2, height2;
    cin >> width2 >> height2;
    double x, y;
    // 假设EOF是结束符，在MacOS中可以使用Ctrl + D来结束输入
    while (cin >> x >> y) {
        vc.push_back(pair<double, double>(x, y));
    }
    poly p(vc, width2, height2);
    
    if (p.isValid()) {
        p.show();
    } else {
        cout << "poly p is invalid";
    }

    return Fl::run();
}

//------------------------------------------------------------------------------
