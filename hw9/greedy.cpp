#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    int w, n;
    cin >> w >> n;

    vector<int> vc(n);
    for (int i = 0; i < n; ++i) {
        cin >> vc[i];
    }
    sort(vc.begin(), vc.end());
    int min_w = vc[0];
    int max_w = vc[n - 1];
    if (min_w <= 0 && max_w > w) {
        cout << "error\n";
        return 1;
    } else if (min_w <= 0) {
        cout << "error\n";
        return 1;
    } else if (max_w > w) {
        cout << "out\n";
        return 1;
    }

    int left = 0, right = n - 1;
    int num = 0;
    while (left < right) {
        if (vc[left] + vc[right] > w) {
            --right;
            ++num;
        } else {
            ++left;
            --right;
            ++num;
        }
    }
    if (left == right) {
        ++num;
    }

    cout << num << endl;

    return 0;
}
