#include <iostream>
#include <vector>

using namespace std;    

int main() {
    int n;
    cin >> n;
    if (n <= 0 || n >= 100001) {
        cout << "error" << endl;
        return 1;
    }
    vector<int> vc(n);
    for (int i = 0; i < n; ++i) {
        cin >> vc[i];
        if (vc[i] < -1000 || vc[i] > 1000) {
            cout << "error" << endl;
            return 1;
        }
    }

    vector<int> dp(n, 0);
    int global_max = vc[0];
    int start = 0;
    int end = 0;
    int curs = 0, cure = 0;

    dp[0] = vc[0];

    for (int i = 1; i < n; ++i) {
        if (dp[i - 1] < 0) {
            dp[i] = vc[i];
            curs = i;
            cure = i;
            if (dp[i] > global_max) {
                global_max = dp[i];
                start = i;
                end = i;
            }
        } else {
            dp[i] = dp[i - 1] + vc[i];
            cure = i;
            if (dp[i] > global_max) {
                global_max = dp[i];
                start = curs;
                end = cure;
            }
        }
    }

    cout << global_max << " " << start + 1 << " " << end + 1 << endl;

    return 0;
}