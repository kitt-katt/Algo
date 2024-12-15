#include <iostream>
#include <string>
using namespace std;

int main() {
    int N, M;
    cin >> N >> M;
    
    string song;
    cin >> song;
    
    int count = 0; // Счетчик, сколько раз фанаты оказались правы

    for (int q = 0; q < M; ++q) {
        int i, j, k;
        cin >> i >> j >> k;
        
        if (song.substr(i - 1, k) == song.substr(j - 1, k)) {
            ++count;
        }
    }
    
    cout << count << '\n';
    
    return 0;
}
