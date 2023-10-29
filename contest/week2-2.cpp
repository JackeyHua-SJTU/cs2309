#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

// find the largest contiguous subarray with equal 0s and 1s
// we can change 0 to -1, then the problem becomes to find the largest contiguous subarray with sum 0
// calculate the prefix sum and use a unordered map to store the first index of each prefix sum
// prefix[i] = sum of nums[0] to nums[i]
// if prefix[i] == prefix[j], then the sum of nums[i + 1] to nums[j] is 0, then the len = j - i 

int main() {
    int n;
    cin >> n; // number of people
    int temp, sum = 0;
    vector<int> prefix(n);
    for (int i = 0; i < n; i++) {
        cin >> temp;
        if (!temp) {
            temp = -1;
        }
        sum += temp;
        prefix[i] = sum;
    }
    int len = 0;
    unordered_map<int, int> mp;
    mp[0] = -1;
    for (int i = 0; i < n; i++) {
        if (mp.count(prefix[i])) {
            len = max(len, i - mp[prefix[i]]);
        } else {
            mp[prefix[i]] = i;
        }
    }
    cout << len;
    return 0;
}