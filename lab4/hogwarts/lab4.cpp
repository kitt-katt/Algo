#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main() {
    string s;
    cin >> s;

    int n = s.size();
    vector<int> pi(n, 0);

    // Расчет префикс-функции
    for (int i = 1; i < n; ++i) {
        int j = pi[i - 1];
        while (j > 0 && s[i] != s[j]) {
            j = pi[j - 1];
        }
        if (s[i] == s[j]) {
            ++j;
        }
        pi[i] = j;
    }

    for (int i = 0; i < n; ++i) {
        cout << pi[i] << " ";
    }
    cout << '\n';

    return 0;
}
