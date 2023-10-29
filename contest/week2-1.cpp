#include <iostream>
#include <vector>
#include <string>

using namespace std;

int index(char c) {
    if (c >= '0' && c <= '9') {
        return c - '0';
    } else {
        return c - 'a' + 10;
    }
}

char getChar(int n) {
    if (n >= 0 && n <= 9) {
        return n + '0';
    } else {
        return n - 10 + 'a';
    }
}

int main() {
    int n;
    cin >> n; // 进制
    string a, b;
    cin >> a >> b;
    string ans;

    int remainder = 0, i = a.size() - 1, j = b.size() - 1;

    while (i >= 0 || j >= 0) {
        if (i < 0) {
            int sum = index(b[j]) + remainder;
            ans = getChar(sum % n) + ans;
            remainder = sum / n;
            j--;
        } else if (j < 0) {
            int sum = index(a[i]) + remainder;
            ans = getChar(sum % n) + ans;
            remainder = sum / n;
            i--;
        } else {
            int sum = index(a[i]) + index(b[j]) + remainder;
            ans = getChar(sum % n) + ans;
            remainder = sum / n;
            i--;
            j--;
        }
    }

    cout << ans;

    return 0;
}