#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {
    string s;
    getline(cin, s);
    int len = s.size();
    /*
     * dp[i][j] means the longest palindrome length in the index range [i, j]
     * str[i][j] means the longest palindrome string in the index range [i, j]
     */
    vector<vector<int>> dp(len, vector<int>(len));
    vector<vector<string>> str(len, vector<string>(len));

    for (int i = len - 1; i >= 0; --i) {
        dp[i][i] = 1;
        str[i][i] = s[i];
        for (int j = i + 1; j < len; ++j) {
            if (s[i] == s[j]) {
                dp[i][j] = dp[i + 1][j - 1] + 2;
                str[i][j] = s[i] + str[i + 1][j - 1] + s[j];
            } else {
                dp[i][j] = max(dp[i + 1][j], dp[i][j - 1]);
                if (dp[i + 1][j] >= dp[i][j - 1]) {
                    str[i][j] = str[i + 1][j];
                } else {
                    str[i][j] = str[i][j - 1];
                }
            }
        }
    }
    if (str[0][len - 1].size() % 2) {
        int l = str[0][len - 1].size() / 2;
        cout << str[0][len - 1].substr(0, l) << str[0][len - 1].substr(l + 1, l) << endl;
    } else {
        cout << str[0][len - 1] << endl;
    }

    return 0;
}
