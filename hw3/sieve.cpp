#include <iostream>
#include <vector>

using namespace std;

vector<int> vc(10000);
vector<int> table(200000);
int pos = 0;

void prime() {
    int i, j;
    for (i = 2; i < 200000; i++) {
        table[i] = i;
    }
    for (i = 2; i < 200000; i++) {
        if (table[i] != 0) {
            vc[pos++] = table[i];
            for (j = i + i; j < 200000; j += i) {
                table[j] = 0;
            }
            if (pos == 10000) {
                break;
            }
        }
    }
}

int main() {
    int lb, ub;
    cin >> lb >> ub;
    
    prime();

    for (int i = lb; i <= ub; i++) {
        cout << vc[i - 1] << " ";
    }

    return 0;
}