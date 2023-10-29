#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// use dfs to traverse all possible situation
// Runtime Error @ 2023/10/29

int number, ans = 0x7fffffff;
vector<pair<int, int>> vc;

void dfs(int curPos, int time, int sum) {
    if (curPos == number) {
        ans = min(ans, sum);
        return;
    }
    if (vc[curPos].first > time) {
        // cout << "Choose " << curPos << ", and its value is " << vc[curPos].second << ", and current time is " << time << endl;
        dfs(curPos + 1, time + 1, sum);
    }
    // cout << "Skip " << curPos << ", and its value is " << vc[curPos].second << endl;
    dfs(curPos + 1, time, sum + vc[curPos].second);
    return ;
}

int main() {
    int money;
    cin >> money >> number;
    vector<int> ddl, fine;
    int input;
    while (cin >> input) {
        ddl.push_back(input);
        if (cin.get() == '\n') {
            break;
        }
    }
    while (cin >> input) {
        fine.push_back(input);
        if (cin.get() == '\n') {
            break;
        }
    }
    
    for (int i = 0; i < number; i++) {
        vc.push_back({ddl[i], fine[i]});
    }
    auto cmp = [](pair<int, int> a, pair<int, int> b) {
        if (a.first < b.first) {
            return true;
        } else if (a.first == b.first) {
            return a.second > b.second;
        } else {
            return false;
        }
    };

    sort(vc.begin(), vc.end(), cmp);

    // for (auto [x, y] : vc) {
    //     cout << x << " " << y << endl;
    // }

    dfs(0, 0, 0);

    cout << money - ans;

    return 0;
}