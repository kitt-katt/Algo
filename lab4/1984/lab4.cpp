#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {
    string T, X;
    cin >> T >> X;

    int count = 0;
    vector<int> indices;

    for (size_t i = 0; i + X.size() <= T.size(); ++i) {
        if (T.substr(i, X.size()) == X) {
            ++count;
            indices.push_back(i);
        }
    }

    cout << count << '\n';
    for (int index : indices) {
        cout << index << " ";
    }
    cout << '\n';

    return 0;
}
