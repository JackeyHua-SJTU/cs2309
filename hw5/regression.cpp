#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>

using namespace std;

double deal(double x) {
    int t = abs(static_cast<int>(x * 1000000));
    // cout << "Now is dealing t " << t << ", remainder is " << t % 1000 << endl;
    if (t % 1000 == 999) {
        t += 1;
        // cout << "change " << x << " to " << t << endl;
        if (x < 0) t *= -1;
        return t / 1000000.0;
    } else {
        return x;
    }
}

int main() {
    double x;
    cin >> x;
    vector<vector<double>> vc;
    double a, b;
    double sumx = 0.0, sumy = 0.0, avrx, avry;
    while (cin >> a >> b) {
        vc.push_back({a, b});
        sumx += a;
        sumy += b;
    }
    int size = vc.size();
    double slope, intercept;
    avrx = sumx / size;
    avry = sumy / size;

    double numerator = 0.0, denominator = 0.0;
    for (int i = 0; i < size; i++) {
        numerator += (vc[i][0] - avrx) * (vc[i][1] - avry);
        denominator += pow(vc[i][0] - avrx, 2);
    }
    slope = numerator / denominator;
    intercept = avry - slope * avrx;

    numerator = 0.0;
    denominator = 0.0;
    for (int i = 0; i < size; i++) {
        denominator += pow(vc[i][1] - avry, 2);
        numerator += pow(avry - slope * vc[i][0] - intercept, 2);
    }

    double r = sqrt(numerator / denominator);
    if (slope < 0) {
        r *= -1;
    }
    if (abs(r) < 0.75) {
        cout << fixed << setprecision(4) << static_cast<int>(deal(r) * 10000) / 10000.0 << endl;
        cout << "error" << endl << "error";
    } else {
        double ans = slope * x + intercept;
        // cout << "debug " << slope << " " << intercept << endl;
        cout << fixed << setprecision(4) << static_cast<int>(deal(r) * 10000) / 10000.0 << endl;
        if (intercept >= 0) {
            cout << fixed << setprecision(4) << "y=" << static_cast<int>(deal(slope) * 10000) / 10000.0 << "*x+" << static_cast<int>(deal(intercept) * 10000) / 10000.0 << endl;
        } else {
            cout << fixed << setprecision(4) << "y=" << static_cast<int>(deal(slope) * 10000) / 10000.0 << "*x" << static_cast<int>(deal(intercept) * 10000) / 10000.0 << endl;
        }
        cout << fixed << setprecision(4) << static_cast<int>(deal(ans) * 10000) / 10000.0;
    }

    return 0;
}