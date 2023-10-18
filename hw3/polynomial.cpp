#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

using namespace std;

class polynomial {
private:
    int maxd;         // the degree of the polynomial
    int mind;
    vector<double> coef;   // the coefficients of the polynomial
    bool error;         // whether the polynomial is valid

    // 检查输入是否合法，不能有连续的字符串，如++, --, +- 等
    void checkValid(string s) {
        if (s[s.size() - 1] == '+' || s[s.size() - 1] == '-' || s[s.size() - 1] == '^') {
            error = true;
            return;
        }
        if (s.find("++") != s.npos) {
            error = true;
            return;
        }
        if (s.find("+-") != s.npos) {
            error = true;
            return;
        }
        if (s.find("-+") != s.npos) {
            error = true;
            return;
        }
        if (s.find("--") != s.npos) {
            error = true;
            return;
        }
        if (s.find("^+") != s.npos) {
            error = true;
            return;
        }
        if (s.find("+^") != s.npos) {
            error = true;
            return;
        }
        if (s.find("-^") != s.npos) {
            error = true;
            return;
        }
        if (s.find("^-") != s.npos) {
            error = true;
            return;
        }
        if (s.find("^^") != s.npos) {
            error = true;
            return;
        }
    }
    // 构造函数调用该函数完成输入的parse
    void parse(string s) {
        error = false;
        if (s.find_first_of("!@#$%&*_|{}[]:;\"'<>,?/") != s.npos) {
            error = true;
            return;
        }
        checkValid(s);
        if (error) {
            return;
        }
        int len = s.size();
        int pos = s.find_first_of("^");
        int maxDeg = 0;
        int minDeg = 0;     // minDeg不再有用，因为多项式的系数一定非负
        // find maximum degree

        // 特判 x 的情况
        int temp = s.find_first_of("x");
        while (temp != s.npos) {
            if (temp + 1 == len || s[temp + 1] == '+' || s[temp + 1] == '-') {
                maxDeg = 1;
                break;
            }
            temp = s.find_first_of("x", temp + 1);
        }

        while (pos != s.npos) {
            int ub = pos + 1;
            if (s[ub] == '(') {
                ub++;
                pos++;
                while (ub < len && s[ub] != ')') {
                    ub++;
                }
            } else {
                while (ub < len && s[ub] != '+' && s[ub] != '-') {
                    ub++;
                }
            }
            maxDeg = max(maxDeg, stoi(s.substr(pos + 1, ub - pos - 1)));
            minDeg = min(minDeg, stoi(s.substr(pos + 1, ub - pos - 1)));
            pos = s.find_first_of("^", ub);
        }
        // initialize the degree and the coefficients
        minDeg = min(minDeg, 0);
        maxd = maxDeg;
        mind = minDeg;
        if (mind < 0) {
            error = true;
            return ;
        }
        coef.resize(maxd + 1, 0.0000);
        /*
            parse 逻辑：
                对于每个 - 前，插入一个+号，两个+号之间的就是子串
        */
        string modifed = "";
        // int minus = 0;
        for (int i = 0; i < len; i++) {
            if (s[i] == '(') {
                while (s[i] != ')') {
                    modifed += s[i++];
                }
            }
            if (s[i] == '-') {
                modifed += '+';
            }
            modifed += s[i];
        }
        if (modifed[0] == '+') {
            pos = modifed.find_first_of('+', 1);
        } else {
            pos = modifed.find_first_of('+');
        }
        while (pos != modifed.npos) {
            helper(modifed.substr(0, pos));
            modifed = modifed.substr(pos);
            if (modifed[0] == '+') {
                pos = modifed.find_first_of('+', 1);
            } else {
                pos = modifed.find_first_of('+');
            }
        }
        helper(modifed);
        
    }
    // parse出元表达书，对每个元表达式进行处理
    void helper(string s) {
        // cout << s << endl;
        double coefficient;
        if (s[0] == '+') {
            s = s.substr(1);
        }
        int positive = 1;
        if (s[0] == '-') {
            positive = -1;
            s = s.substr(1);
        }
        int p = s.find_first_of('x');
        if (p == s.npos) {
            coef[0] += positive * stod(s);
        } else {
            if (p == 0) {
                coefficient = 1.0000;
            } else {
                coefficient = stod(s.substr(0, p));
            }
            if (p == s.size() - 1) {
                coef[1] += positive * coefficient;
            } else if (p + 2 < s.size()) {
                int deg;
                if (s[p + 2] == '(') {
                    deg = stoi(s.substr(p + 3, s.size() - p - 4));
                } else {
                    deg = stoi(s.substr(p + 2));
                }
                coef[deg] += positive * coefficient;
            } else {
                error = true;
            }
        }
    }

    
public:
    polynomial(string s) {
        parse(s);
    }

    polynomial(vector<double> v, int maxd) {
        this->maxd = maxd;
        this->coef = v;
        this->error = false;
    }

    polynomial() {
        this->error = true;
    }
    
    // 修正多项式的最高次数和最低次数
    void modify() {
        int i = coef.size() - 1;
        while (i >= 0 && coef[i] == 0) {
            i--;
        }
        if (i == -1) {
            maxd = 0;
        } else {
            maxd = i;
            coef = vector<double>(coef.begin(), coef.begin() + i + 1);
        }
    }
    // 检查是否全0
    bool zero(vector<double> v) {
        for (auto i : v) {
            if (i != 0) {
                return false;
            }
        }
        return true;
    }

    polynomial operator+(const polynomial &p) {
        if (this->error || p.error) {
            return polynomial();
        }
        int maxd = max(this->maxd, p.maxd);
        // int mind = min(this->mind, p.mind);
        vector<double> vc(maxd + 1, 0.0000);
        for (int i = 0; i <= this->maxd; i++) {
            vc[i] += this->coef[i];
        }
        for (int i = 0; i <= p.maxd; i++) {
            vc[i] += p.coef[i];
        }
        return polynomial(vc, maxd);
    }

    polynomial operator-(const polynomial &p) {
        if (this->error || p.error) {
            return polynomial();
        }
        int maxd = max(this->maxd, p.maxd);
        // int mind = min(this->mind, p.mind);
        vector<double> vc(maxd + 1, 0.0000);
        for (int i = 0; i <= this->maxd; i++) {
            vc[i] += this->coef[i];
        }
        for (int i = 0; i <= p.maxd; i++) {
            vc[i] -= p.coef[i];
        }

        return polynomial(vc, maxd);
    }

    polynomial operator*(const polynomial &p) {
        if (this->error || p.error) {
            return polynomial();
        }
        int maxd = this->maxd + p.maxd;
        // int mind = this->mind + p.mind;
        vector<double> vc(maxd + 1, 0.0000);
        for (int i = 0; i < this->coef.size(); i++) {
            for (int j = 0; j < p.coef.size(); j++) {
                vc[i + j] += this->coef[i] * p.coef[j];
            }
        }
        return polynomial(vc, maxd);
    }

    vector<polynomial> operator/(const polynomial &p) {
        if (this->error || p.error) {
            return vector<polynomial>{polynomial(), polynomial()};
        } 
        if (zero(p.coef) || this->maxd < p.maxd) {
            return vector<polynomial>{polynomial(), polynomial()};
        }
        int maxd = this->maxd - p.maxd;
        vector<double> vc(maxd + 1, 0.0000);
        polynomial cp = *this;

        for (int i = maxd; i >= 0; i--) {
            vc[i] = cp.coef[i + p.maxd] / p.coef[p.maxd];
            for (int j = 0; j <= p.maxd; j++) {
                cp.coef[i + j] -= vc[i] * p.coef[j];
            }
        }
        polynomial q(vc, maxd);
        polynomial a = *this - q * p;
        vector<polynomial> res = {q, a};
        // {商，余数}
        return res;
    }

    ostream& operator<<(ostream& os) {
        this->modify();
        if (this->error) {
            os << "error";
            return os;
        }
        os << fixed << setprecision(4);
        if (this->maxd == 0 && this->coef[0] == 0.0) {
            os << "0.0000";
        }
        for (int i = maxd; i > 1; i--) {
            if (i == maxd) {
                os << static_cast<int>(this->coef[i] * 10000) / 10000.0 << "x^" << (int)i;
                continue;
            }
            if (this->coef[i] > 0) {
                os << "+" << static_cast<int>(this->coef[i] * 10000) / 10000.0 << "x^" << (int)i;
            } else if (this->coef[i] < 0){
                os << static_cast<int>(this->coef[i] * 10000) / 10000.0 << "x^" << (int)i;
            }
        }

        if (this->coef[1] != 0) {
            if (this->coef[1] > 0 && maxd > 1) {
                os << "+" << static_cast<int>(this->coef[1] * 10000) / 10000.0 << "x";
            } else if((this->coef[1] < 0) || maxd == 1){
                os << static_cast<int>(this->coef[1] * 10000) / 10000.0 << "x";
            }
        }

        if (this->coef[0] != 0) {
            if (this->coef[0] > 0 && maxd > 0) {
                os << "+" << static_cast<int>(this->coef[0] * 10000) / 10000.0;
            } else if (this->coef[0] < 0 || maxd == 0){
                os << static_cast<int>(this->coef[0] * 10000) / 10000.0;
            }
        }
        return os;
    }

    void getCoef() {
        for (auto i : coef) cout << i << " ";
        cout << endl;
    }

};

int main() {
    string s1, s2;
    cin >> s1 >> s2;
    polynomial p1(s1);
    polynomial p2(s2);
    p1 << cout << endl;
    p2 << cout << endl;
    p1 + p2 << cout << endl;
    p1 - p2 << cout << endl;
    p1 * p2 << cout << endl;
    auto v = p1 / p2;
    v[0] << cout << endl;
    v[1] << cout << endl;
    return 0;
}