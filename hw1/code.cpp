// @author 花振东 521021910653
// @date 2023.9.20
// @details coding homework 1 on educoder
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <set>

using namespace std;

// 解析文件名，返回文件名和格式
pair<string, string> parseName(string filename) {
    string name, format;
    for (auto i = filename.size() - 1; i >= 1; i--) {
        if (filename[i] == '.') {
            name = filename.substr(0, i);
            format = filename.substr(i + 1);
            break;
        }
    }
    return make_pair(name, format);
}

// 检查文件是否以 副本x 结尾
bool checkDup(string filename) {
    pair<string, string> p = parseName(filename);
    string name = p.first;

    for (int i = p.first.size() - 1; i >= 0; i--) {
        if (p.first[i] == '_') {
            return true;
        }
    }
    return false;
}

// 对于 确定 是副本的文件，返回其源文件名和版本号
pair<string, int> sourceName(string filename) {
    pair<string, string> p = parseName(filename);
    string name = p.first;
    int version = 0;
    // wcout << "current name : " << name << endl;
    for (auto i = name.size() - 1; i >= 0; i--) {
        if (name[i] == '_') {
            // cout << "i : " << i << "name.size() = " << name.size() << endl;
            if (i + 7 < name.size()) {
                // wcout << "i : " << i << "substr : " << name.substr(i + 7) << endl;
                version = stoi(name.substr(i + 7));
            }
            name = name.substr(0, i);
            break;
        }
    }
    return make_pair(name, version);
}

// 检查当前文件名下所有副本的最新版本号
void getVersion(set<string>& src, map<string, int> &version) {
    for (auto i : src) {
        // 只有已经是副本的文件需要记录版本号
        if (checkDup(i)) {
            pair<string, int> p = sourceName(i);
            string name = p.first;
            int ver = p.second;
            if (version.find(name) == version.end()) {
                // wcout << "map " << name << " " << ver << endl;
                version[name] = ver;
            } else {
                // wcout << "map " << name << " " << ver << endl;
                version[name] = max(version[name], ver);
            }
        }
    }
}

// 拼接文件名
string concat(string filename, int version = 0) {
    string ans;
    if (version == 0) {
        return parseName(filename).first + "_副本." + parseName(filename).second;
    }
    string ver = to_string(version);
    return sourceName(filename).first + "_副本" + ver + "." + parseName(filename).second;
}

int main() {
    set<string> file;
    vector<string> cpList;
    map<string, int> mp;
    string input;

    while (cin >> input) {
        file.insert(input);
        if (cin.get() == '\n') {
            break;
        }
    }

    while (cin >> input) {
        cpList.emplace_back(input);
        if (cin.get() == '\n') {
            break;
        }
    
    getVersion(file, mp);

    for (auto i : cpList) {
        // cout << "current : " << i << " ";
        if (checkDup(i)) {
            if (!file.count(i)) {
                // cout << " go wrong. \n";
                continue;
            } else {
                int ver = (++mp[sourceName(i).first]);
                file.insert(concat(i, ver));
                // cout << concat(i, ver) << "\n";
            }
        } else {
            if (!file.count(i)) {
                // cout << " go wrong. \n";
                continue;
            } else {
                if (mp.count(parseName(i).first)) {
                    int ver = (++mp[parseName(i).first]);
                    file.insert(concat(i, ver));
                    // cout << concat(i, ver) << "ver : " << ver <<  "\n";
                } else {
                    file.insert(concat(i));
                    // cout << concat(i) << "\n";
                }
            }
        }
    }

    for (auto i : file) {
        cout << i << "\n";
    }


    return 0;
}
