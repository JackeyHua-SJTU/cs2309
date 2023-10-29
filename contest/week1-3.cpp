#include <iostream>
#include <vector>
#include <climits> // INT_MAX or we can use 0x7fffffff

using namespace std;

int ans = INT_MAX;
vector<vector<int>> vc;

// traverse all possible selections of dishes
// if the dish is not chosen, then the sourness and bitterness remain the same, abandon it
void dfs(int curPos, int sour, int bitter) {
    if (curPos == vc.size()) {
        if (sour != 1) {
            // does not choose any dishes
            ans = min(ans, abs(sour - bitter));
        }
        return;
    }
    dfs(curPos + 1, sour, bitter);
    dfs(curPos + 1, sour * vc[curPos][0], bitter + vc[curPos][1]);
    return ;
}

int main() {
    int n;
    cin >> n;
    
    for (int i = 0; i < n; i++) {
        int sour, bitter;
        cin >> sour >> bitter;
        vc.push_back({sour, bitter});
    }
    int sour = 1, bitter = 0;
    dfs(0, sour, bitter);
    cout << ans;

    return 0;
}