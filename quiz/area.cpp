// quiz @ 2023-10-25
// calculate the area of polygon

#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

using namespace std;

double space(double x1, double y1, double x2, double y2) {
    return (x1 * y2 - x2 * y1) / 2;
}

int main() {
    vector<vector<double>> vc;
    double a, b;
    while (cin >> a >> b) {
        vc.push_back({a, b});
    }
    double ans = 0.0;
    
    int size = vc.size();
    for (int i = 0; i < size - 1; i++) {
        ans += space(vc[i][0], vc[i][1], vc[i + 1][0], vc[i + 1][1]);
    }
    ans += space(vc[size - 1][0], vc[size - 1][1], vc[0][0], vc[0][1]);

    cout << fixed << setprecision(4) << abs(ans) << endl;
    return 0;
}