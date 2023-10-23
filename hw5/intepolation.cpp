#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;

int main() {
    double x;
    cin >> x;
    vector<vector<double>> vc;
    double a, b;
    while (cin >> a >> b) {
        vc.push_back({a, b});
    }

    double ans = 0.0;
    int size = vc.size();
    for (int i = 0; i < size; i++) {
        double s = vc[i][1];
        for (int j = 0; j < size; j++) {
            if (j != i) {
                s *= (x - vc[j][0]) / (vc[i][0] - vc[j][0]);
            }
        }
        ans += s;
    }
    
    if (ans == 0.0) {
        cout << "0.0000" << endl;
    } else {
        int temp = static_cast<int>(ans * 10000000);
        if (temp % 1000 == 999) {
            temp += 1;
            cout << fixed << setprecision(4) << temp / 10000000.0 << endl;
        } else {
            cout << fixed << setprecision(4) << static_cast<int>(ans * 10000.0) / 10000.0 << endl;
        }
        
    }

    return 0;
}