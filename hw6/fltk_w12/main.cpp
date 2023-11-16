#include "superellipse.h"
#include "poly.h"
#include <FL/Fl.H>
#include <iostream>

using std::vector;
using std::pair;
using std::cout;
using std::endl;
using std::cin;

//------------------------------------------------------------------------------


// ! 在运行本文件前，请先看当前文件夹下的README.md


int main() {
//----------------------------------TASK1----------------------------------------
    superellipse s1(300, 200, 2, 2, 10, 5, 800, 600);    // 指定k为5
    superellipse s2(300, 200, 2, 2, 10, 800, 600);       // 不指定k 

    // 如果采用命令行输入，那么请注释掉上面一行，并恢复下面的输入代码

    // double a, b, m, n;
    // int N, K;
    // cin >> a >> b >> m >> n >> N >> K;
    // superellipse s(a, b, m, n, N, K, 800, 600);
    
    s1.show();
    s2.show();

//---------------------------------TASK2-----------------------------------------

    vector<pair<double, double>> valid_points = {{0, 0}, {100, 0}, {100, 100}, {0, 100}};

    vector<pair<double, double>> invalid_points = {{0, 0}, {100, 0}, {100, 100}, {0, -100}};
    
    poly p1(valid_points, 800, 600);
    poly p2(invalid_points, 800, 600);

    if (p1.isValid()) {
        p1.show();
    } else {
        cout << "poly p1 is not valid\n"; 
    }

    if (p2.isValid()) {
        p2.show();
    } else {
        cout << "poly p2 is not valid\n";
    }

    // 如果采用命令行输入，那么请注释掉上面那段代码，并恢复下面的输入代码

    // vector<pair<double, double>> vc;
    // double x, y;
    // // 假设EOF是结束符，在MacOS中可以使用Ctrl + D来结束输入
    // while (cin >> x >> y) {
    //     vc.push_back(pair<double, double>(x, y));
    // }
    // poly p(vc, 800, 600);
    
    // if (p.isValid()) {
    //     p.show();
    // } else {
    //     cout << "poly p is invalid";
    // }

    return Fl::run();
}

//------------------------------------------------------------------------------
