#include <iostream>
#include <vector>
#include <chrono>
#include "window.h"
#include <FL/Fl.H>

using std::vector;
using std::pair;
using namespace std::chrono;

int main() {
    vector<pair<double, double>> points = {{100.0, 100.0}, {150.0, 300.0}, {170.0, 130.0}, {300.0, 130.0}, {320.0, 300.0}, {350.0, 130.0}, {450.0, 130.0}, {470.0, 300.0}, {500.0, 100.0}};

    vector<pair<double, double>> p1 = {{100.0, 100.0}, {500.0, 100.0}, {400.0, 220.0}, {600.0, 310.0}, {600.0, 200.0}, {700.0, 300.0}, {800.0, 500.0}, {490.0, 500.0}, {700.0, 700.0}, {400.0, 600.0}, {300.0, 300.0}};
    vector<pair<double, double>> o1 = {{400, 350}, {400, 450}, {600, 450}, {600, 350}};

    vector<pair<double, double>> p2 = {{10, 10}, {30, 10}, {30, 30}, {10, 30}};
    vector<pair<double, double>> o2 = {{15, 15}, {25, 15}, {25, 25}, {15, 25}};

    vector<pair<double, double>> p3 = {{100, 100}, {300, 300}, {300, 100}};
    vector<pair<double, double>> o3 = {{200, 200}, {200, 150}, {250, 150}, {250, 200}};

    vector<pair<double, double>> p4 = {{10.0, 10.0}, {50.0, 10.0}, {40.0, 22.0}, {60.0, 31.0}, {60.0, 20.0}, {70.0, 30.0}, {80.0, 50.0}, {49.0, 50.0}, {70.0, 70.0}, {40.0, 60.0}, {30.0, 30.0}};
    vector<pair<double, double>> p5 = {{10.0, 10.0}, {13.0, 20.0}, {15.0, 12.0}, {17.0, 12.0}, {20.0, 20.0}, {22.0, 12.0}, {24.0, 12.0}, {27.0, 20.0}, {29.0, 10.0}};


    auto start = high_resolution_clock::now();
    // window w(1000, 800, p1, 1, "poly");
    // window w(1000, 800, points, 1, "poly");
    // window w(1000, 800, p1, o1, 1, "poly");
    window w(1000, 800, points, 2, "poly");
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    std::cout << "time taken : " << duration.count() << std::endl;
    // w.p.add_curve({100.0, 100.0}, {200.0, 200.0}, {200.0, 100.0});
    w.show();
    return Fl::run();
}