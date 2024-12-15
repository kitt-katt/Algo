#include <iostream>
#include <vector> 
#include <algorithm>
#include <string>

using namespace std;

struct Fenwick {
    // структура данных Fenwick Tree для динамического подсчёта префиксных сумм
    int n;
    vector<int> fenw;
    Fenwick(int n): n(n), fenw(n+1,0) {}
    
    void update(int i, int delta) {
        for (; i <= n; i += i & (-i)) {
            fenw[i] += delta;
        }
    }
    
    int query(int i) {
        //  вычисление префиксной суммы от 1 до i
        int s = 0;
        for (; i > 0; i -= i & (-i)) {
            s += fenw[i];
        }
        return s;
    }
    
    // Возвращает минимальный индекс pos, такой что префикс fenw до pos >= k
    int find_kth(int k) {
        int pos = 0;
        int bit_mask = 1 << (31 - __builtin_clz(n)); 
        // ищем индекс бинраным поиском по Fenw
        for (; bit_mask > 0; bit_mask >>= 1) {
            int next_pos = pos + bit_mask;
            if (next_pos <= n && fenw[next_pos] < k) {
                k -= fenw[next_pos];
                pos = next_pos;
            }
        }
        return pos + 1;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N; 
    cin >> N;
    
    vector<string> queries_str; // хранит имена книг
    vector<long long> queries_k; // хранит значения 𝑘 для запросов. 
    // Если запрос связан с именем, в queries_k записываем −1
    
    queries_str.reserve(N);
    queries_k.reserve(N);

    vector<string> names; 
    names.reserve(N);

    for (int i = 0; i < N; i++) {
        string q; cin >> q;
        if (isdigit(q[0])) {
            queries_str.push_back("");
            queries_k.push_back(stoll(q));
        } else {
            queries_str.push_back(q);
            queries_k.push_back(-1);
            names.push_back(q);
        }
    }

    // Сортируем массив names и удаляем дубликаты для создания уникальных индексов
    sort(names.begin(), names.end());
    names.erase(unique(names.begin(), names.end()), names.end());
    
    // Создаём дерево Фенвика размером 𝑀 — количество уникальных имён.
    int M = (int)names.size();
    Fenwick fenw(M);

    for (int i = 0; i < N; i++) {
        if (queries_k[i] == -1) {
            // Вставка имени
            string &nm = queries_str[i];
            int id = (int)(lower_bound(names.begin(), names.end(), nm) - names.begin()) + 1;
            fenw.update(id, 1);
        } else {
            // Запрос k
            int k = (int)queries_k[i];
            int id = fenw.find_kth(k);
            cout << names[id-1] << "\n";
        }
    }

    return 0;
}
