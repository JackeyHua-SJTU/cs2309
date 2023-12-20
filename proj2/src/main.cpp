#include <iostream>
#include <vector>
#include <chrono>
#include "window.h"
#include <FL/Fl.H>

using std::vector;
using std::pair;
using namespace std::chrono;

int main() {
    // vector<pair<double, double>> points = {{100.0, 100.0}, {150.0, 300.0}, {170.0, 130.0}, {300.0, 130.0}, {320.0, 300.0}, {350.0, 130.0}, {450.0, 130.0}, {470.0, 300.0}, {500.0, 100.0}};
    // vector<pair<double, double>> points = {{0.0, 0.0}, {50.0, 50.0}, {70.0, 30.0}, {100.0, 30.0}, {120.0, 50.0}, {150.0, 0.0}};
    
    vector<pair<double, double>> points = {{100.0, 100.0}, {500.0, 100.0}, {400.0, 220.0}, {600.0, 310.0}, {600.0, 200.0}, {700.0, 300.0}, {800.0, 500.0}, {490.0, 500.0}, {700.0, 700.0}, {400.0, 600.0}, {300.0, 300.0}};
    auto start = high_resolution_clock::now();
    window w(1000, 800, points, "poly");
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    std::cout << "time taken : " << duration.count() << std::endl;
    // w.p.add_curve({100.0, 100.0}, {200.0, 200.0}, {200.0, 100.0});
    w.show();
    return Fl::run();
    /*
        0 0
50 50
70 30
96.8182 30
100 30
120 50
121.85 46.9163
150 0
    
    */

    // poly p(points);
    // // std::cout << p.area({69.0, 6.0}) << std::endl;
    // auto s = p.get_vertex_set({173.0, 100.0});
    // for (auto&& [x, y] : s) {
    //     std::cout << x << " " << y << std::endl;
    // }
    // // auto area = p.area_helper(s);
    // // std::cout << "area : " << area << std::endl;
    // // for (auto&& i : p.get_vertex_set({169.0, 106.0})) {
    // //     std::cout << i.first << " " << i.second << std::endl;
    // // }
    // // p.execute();
    return 0;
}