#include <iostream>
#include <vector>
#include <string>
#include <map>

using namespace std;

// corner case #8: * is still a sign.
string sign = ".,!:;()<>?/\\\"\'*&^{}%";

bool isWord(string input) {
    if (input.size() == 1 && (input[0] == '-' || input[0] == '_')) {
        return false;
    }
    return (input.find_first_of(sign) == input.npos);
}

string upper_lower_reverse(string input) {
    for (int i = 0; i < input.size(); i++) {
        if (input[i] >= 'A' && input[i] <= 'Z') {
            input[i] += 32;
        } 
    }
    return input;
}

pair<string, bool> dealing(string input) {
    // int pos = input.find('_');
    int len = input.size();
    // 不存在 _ 
    // if (pos == input.npos) {
    //     return {input, true};
    // }
    // 存在 _ 但是后面全是空格
    if (len == 1 && input[0] == '_') {
        return {{}, false};
    }
    if (len == 1 && input[0] == '-') {
        return {{}, false};
    }
    if (len > 1 && (input[0] == '_' || input[0] == '-')) {
        input = input.substr(1);
        len -= 1;
    }
    if (input.find_first_of(sign, len - 1) != input.npos || input[len - 1] == '_' || input[len - 1] == '-') {
        return {input.substr(0, len - 1), true};
    }
    return {input, true};
}


int main() {
    string input;
    vector<string> bank;
    vector<string> vc;
    map<string, int> mp;
    while (cin >> input) {
        bank.emplace_back(input);
        if (cin.get() == '\n') {
            break;
        }
    }

    while (cin >> input) {
        vc.emplace_back(input);
        if (cin.get() == '\n') {
            break;
        }
    }

    for (auto i : bank) {
        auto cur = dealing(i);
        if (!cur.second) {
            continue;
        } else {
            string original = cur.first;
            string temp;
            int cur = original.find_first_of(sign);
            int prev = 0;
            original = upper_lower_reverse(original);

            if (cur == original.npos) {
                if (!mp.count(original)) {
                        mp[original] = 1;
                } else {
                        mp[original]++;
                } 
                continue;
            }

            while (cur != original.npos && prev < original.size()) {
                if (cur != prev) {
                    temp = original.substr(prev, cur - prev);
                    if (!mp.count(temp)) {
                        mp[temp] = 1;
                    } else {
                        mp[temp]++;
                    }
                }
                prev = cur + 1;
                cur = original.find_first_of(sign, cur + 1);
            }
            if (prev < original.size()) {
                temp = original.substr(prev);
                    if (!mp.count(temp)) {
                        mp[temp] = 1;
                    } else {
                        mp[temp]++;
                    } 
            }
        }
    }

    for (auto i  : vc) {
        if (isWord(i)) {
            int freq = 0;
            string real = upper_lower_reverse(dealing(i).first);
            if (mp.count(real)) {
                freq = mp[real];
            } 
            cout << "(" << i << "," << freq << ")" << ' ';
        }
        
    }

    return 0;
}