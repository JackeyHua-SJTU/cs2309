#include <iostream>
#include <iomanip>
#include <regex>
#include <vector>
#include <string>
#include <set>
#include <unordered_map>

using namespace std;

class linearEquation {
private:
    // Ax = b
    vector<vector<double>> coefficients;    // A, coeffiecient matrix
    vector<double> constants;   // b, constant vector
    set<string> variables;  // x, variables, temporary
    unordered_map<string, int> mp; // map variable to index
    vector<string> equations;   // input equations
    vector<string> dealedEquations; // preprocessed equations
    vector<int> validEquations; // INDEX of valid equations
    vector<double> ans;    // answer
    bool error1;    // no sol
    bool error2;    // infinite sol

    string preprocess(const string& s) {
        string ans = "";
        for (int i = 0; i < s.length(); ++i) {
            if (s[i] == '-') {
                ans += "+-";
            } else {
                ans += s[i];
            }
        }
        if (s[0] != '-') {
            ans = '+' + ans;
        }
        return ans;
    }

    bool validity(const string& s) {
        regex pattern1 (R"((\+\-?([0-9]+(.[0-9]+)?\*)?)([a-z])(0|[1-9][0-9]?))");
        regex pattern2 (R"(=((\+\-)?[0-9]+(.[0-9]+)?))");
        sregex_iterator it (s.begin(), s.end(), pattern1);
        sregex_iterator end;
        int pos = it->position();
        string prev = it->str();
        ++it;

        // 参数部分必须连续
        while (it != end) {
            if (it->position() != pos + prev.length()) {
                return false;
        }
        prev = it->str();
        pos = it->position();
        ++it;
        }
        // cout << s.substr(pos + prev.length()) << endl;
        // 最后一个参数后面的子串必须满足 =number的格式
        if (!regex_match(s.substr(pos + prev.length()), pattern2)) {
            return false;
        }
        return true;
    }

    void getVariables(const string& s) {
        regex pattern (R"([a-z](0|[1-9][0-9]?))");
        sregex_iterator it (s.begin(), s.end(), pattern);
        sregex_iterator end;

        while (it != end) {
            variables.insert(it->str());
            ++it;
        }
    }

    void setToMap() {
        int i = 0;
        for (auto& s : variables) {
            mp[s] = (i++);
        }
    }
    
    // parse equation into coefficients and constants
    void parseCoeff(const string& s) {
        vector<double> curCoefficients(variables.size(), 0.0);
        regex pattern(R"(\+(\-?([0-9]+(.[0-9]+)?\*)?)([a-z](0|[1-9][0-9]?)))");

        smatch match;
        sregex_iterator it(s.begin(), s.end(), pattern);
        sregex_iterator end;

        while (it != end) {
            string temp = it->str();
            regex_search(temp, match, pattern);
            auto name = match[4].str(), coef = match[1].str();
            double coeff;
            if (coef.size() == 0) {
                coeff = 1.0;
            } else if (coef.size() == 1 && coef[0] == '-') {
                coeff = -1.0;
            } else {
                coeff = stod(coef.substr(0, coef.size() - 1));
            }
            curCoefficients[mp[name]] = coeff;
            ++it;
        }
        coefficients.push_back(curCoefficients);
    }

    void parseConst(const string& s) {
        regex pattern (R"(=((\+\-)?[0-9]+(.[0-9]+)?))");
        smatch match;
        regex_search(s, match, pattern);
        auto constant = match[1].str();
        if (constant[0] == '+') {
            constant = constant.substr(1);
        }
        constants.push_back(stod(constant));
    }

    void solve() {
        error1 = false;
        error2 = false;
        int n = coefficients.size(); // row
        if (n == 0) {
            return ;
        }
        int m = coefficients[0].size(); // column
        if (n < m) {
            error2 = true;
            return;
        }
        vector<vector<double>> augmentedMatrix(n, vector<double>(m + 1, 0.0));  // 增广矩阵
        for (int i = 0; i < n; i ++) {
            for (int j = 0; j < m; j ++) {
                augmentedMatrix[i][j] = coefficients[i][j];
            }
            augmentedMatrix[i][m] = constants[i];
        }

        for (int i = 0; i < n; i ++) {
            // 保证对角元非0，如果是0，说明当前列全是0
            if (augmentedMatrix[i][i] == 0) {
            int nonZeroRow = -1;
            for (int j = i + 1; j < n; j++) {
                if (augmentedMatrix[j][i] != 0) {
                    nonZeroRow = j;
                    break;
                }
            }
            if (nonZeroRow == -1) {
                // 此时系数的行列式为0，方程组要么无穷解，要么无解，继续处理下去
                continue;
            }
            swap(augmentedMatrix[i], augmentedMatrix[nonZeroRow]);
            }
            // 此时对角元非0，可以进行消元
            double pivot = augmentedMatrix[i][i];
            for (int j = i; j <= m; j++) {
                augmentedMatrix[i][j] /= pivot;
            }

            // 消元操作
            for (int j = 0; j < n; j++) {
                if (j != i) {
                    double factor = augmentedMatrix[j][i];
                    for (int k = i; k <= m; k++) {
                        augmentedMatrix[j][k] -= factor * augmentedMatrix[i][k];
                    }
                }
            }
        }
        for (int i = 0; i < m; i ++) {
            if (augmentedMatrix[i][i] == 0 && augmentedMatrix[i][m] != 0) {
                error1 = true;
                return;
            } else if (augmentedMatrix[i][i] == 0 && augmentedMatrix[i][m] == 0) {
                error2 = true;
                return;
            } else {
                ans.push_back(augmentedMatrix[i][m]);
            }
        }        
    }

public:
    linearEquation() = default;

    void addEquation(const string& equation) {
        equations.push_back(equation);
        dealedEquations.push_back(preprocess(equation));
    }

    void sol() {
        for (int i = 0; i < equations.size(); ++i) {
            if (validity(dealedEquations[i])) {
                validEquations.push_back(i);
                getVariables(dealedEquations[i]);
            }
        }
        setToMap();
        for (auto i : validEquations) {
            parseCoeff(dealedEquations[i]);
            parseConst(dealedEquations[i]);
        }
        solve();
        cout << "Legal Equations:" << endl;
        if (validEquations.size() > 0) {
            for (auto i : validEquations) {
                cout << equations[i] << endl;
            }
            if (error1) {
                cout << "error1" << endl;
            } else if (error2) {
                cout << "error2" << endl;
            } else {
                cout << fixed << setprecision(4);
                for (auto i : variables) {
                    double answer = ans[mp[i]];
                    if (answer != 0.0) {
                        int remainder = static_cast<int>(answer * 10000) % 10000;
                        if (remainder == 9999) {
                            answer += 0.0001;
                        }
                        cout << i << ":" << static_cast<int>(answer * 10000) / 10000.0 << endl;
                        // cout << i << ":" << ans[mp[i]] << endl;
                    } else {
                        cout << i << ":" << "0.0000" << endl;
                    }
                }
            }
        }
        cout << "Illegal Equations:" << endl;
        for (int i = 0; i < equations.size(); i++) {
            if (find(validEquations.begin(), validEquations.end(), i) == validEquations.end()) {
                cout << equations[i] << endl;
            }
        }
        
    }

};

int main() {
    string input;
    linearEquation le;
    while (getline(cin, input)) {
        // if (input.size() == 0) {
        //     break;
        // }
        le.addEquation(input);
    }
    le.sol();
    return 0;
}