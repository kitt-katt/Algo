#include <iostream>
#include <vector>
using namespace std;


const int MAX_DISPLACEMENTS = 1000000; // Максимальное количество перемещений при вставке
const int MAX_REHASH_ATTEMPTS = 1000000; // Максимальное количество перехеширований


// Класс для управления хэш-функциями
class HashFunctions {
private:
    long long prime1, prime2, mod;
public:
    HashFunctions(long long p1 = 101, long long p2 = 37, long long m = 400003)
    : prime1(p1), prime2(p2), mod(m) {}

    // Первая хэш-функция
    size_t hash1(const string& str) const {
        long long h = 0;
        for (char c : str) {
            h = (h * prime1 + c) % mod;
        }
        return (h + mod) % mod;
    }

    // Вторая хэш-функция
    size_t hash2(const string& str) const {
        long long h = 0;
        for (char c : str) {
            h = (h * prime2 + c) % mod;
        }
        return (h + mod) % mod;
    }

    // Смена параметров хэширования
    void randomize() {
        prime1 = 1009 + rand() % 1000007;
        prime2 = 10007 + rand() % 1000007;
    }
};


// Структура для хранения элемента и его счетчика
struct Entry {
    string key;
    int count;
    bool occupied;

    Entry() : key(""), count(0), occupied(false) {}
    Entry(const string &k, int c) : key(k), count(c), occupied(true) {}
};

// Класс Хеш-таблицы Кукушки с двумя таблицами
class CuckooHashTable {
private:
    size_t size;
    vector<Entry> table1;
    vector<Entry> table2;
    HashFunctions hf;

    // Вектор всех элементов для перехеширования
    vector<pair<string, int>> elements;

public:
    CuckooHashTable(size_t expected_elements) {
        // Размер таблицы выбирается как ближайшая степень двойки больше 2 * expected_elements
        // size = 1;
        // while (size < 2 * expected_elements) size <<= 1;
        size = expected_elements;
        table1.resize(size, Entry());
        table2.resize(size, Entry());
        srand(time(0));
    }

    // Функция хэширования
    size_t hash(int func, const string &key) const {
        switch (func)
        {
            case 1:
                return hf.hash1(key) % size;
            case 2:
                return hf.hash2(key) % size;
            default:
                return 0.0;
        }
    }

    // Вставка элемента с учетом возможного зацикливания
    bool insert(const string &key, const int &val = 1) {
        // Проверяем первую таблицу
        size_t pos1 = hf.hash1(key) % size;
        if (table1[pos1].occupied) {
            if (table1[pos1].key == key) {
                table1[pos1].count += val;
                return true;
            }
        } else {
            table1[pos1] = Entry(key, val);
            elements.emplace_back(key, val);
            return true;
        }

        // Проверяем вторую таблицу
        size_t pos2 = hf.hash2(key) % size;
        if (table2[pos2].occupied) {
            if (table2[pos2].key == key) {
                table2[pos2].count += val;
                return true;
            }
        } else {
            table2[pos2] = Entry(key, val);
            elements.emplace_back(key, val);
            return true;
        }

        // Начинаем процесс замены (кукушки)
        Entry tmp = Entry(key, val);
        int displacement = 0;

        while (displacement < MAX_DISPLACEMENTS) {
            // Пытаемся вставить в первую таблицу
            pos1 = hf.hash1(key) % size;
            if (!table1[pos1].occupied) {
                table1[pos1] = tmp;
                elements.emplace_back(tmp.key, tmp.count);
                return true;
            }
            swap(tmp, table1[pos1]);

            // Пытаемся вставить в вторую таблицу
            pos2 = hf.hash2(key) % size;
            if (!table2[pos2].occupied) {
                table2[pos2] = tmp;
                elements.emplace_back(tmp.key, tmp.count);
                return true;
            }
            swap(tmp, table2[pos2]);

            displacement++;
        }

        // Зацикливание: необходимо перехеширование
        return false;
    }

    void rehash() {
        int attempt = 0;

        // Собираем все элементы из обеих таблиц
        vector<pair<string, int>> old_elements;

        for (const auto &entry : table1) {
            if (entry.occupied) old_elements.emplace_back(entry.key, entry.count);
        }
        for (const auto &entry : table2) {
            if (entry.occupied) old_elements.emplace_back(entry.key, entry.count);
        }

        // Цикл для повторного перехеширования с ограничением по числу попыток
        while (attempt <= MAX_REHASH_ATTEMPTS) {
            // Очистка таблиц перед вставкой новых элементов
            fill(table1.begin(), table1.end(), Entry());
            fill(table2.begin(), table2.end(), Entry());

            // Перегенерация хеш-функций
            hf.randomize();

            // Перебираем все старые элементы и пытаемся вставить их заново
            bool all_inserted = true;  // Флаг, чтобы отслеживать, удачно ли вставлены все элементы

            for (auto &[key, cnt] : old_elements) {
                bool success = insert(key, cnt);  // Пытаемся вставить элемент
                if (!success) {
                    all_inserted = false;  // Если хотя бы один элемент не вставился, нужно повторить перехеширование
                    break;  // Прерываем цикл, чтобы начать новый раунд перехеширования
                }
            }

            // Увеличиваем счетчик попыток
            attempt++;

            if (attempt > MAX_REHASH_ATTEMPTS) {
                cerr << "Too many rehash attempts, something went wrong!" << endl;
                return;  // Если слишком много попыток, прекращаем выполнение
            }
        }
    }


    // Основной метод вставки с обработкой зацикливания
    void insert_with_rehash(const string &key) {
        bool success = insert(key);
        if(!success) {
            // В случае зацикливания выполняем перехеширование
            rehash();
        }
    }

    // Функция для поиска и получения количества вхождений ключа
    int find_count(const string &key) const {
        size_t pos1 = hf.hash1(key) % size;
        if (table1[pos1].occupied && table1[pos1].key == key) {
            return table1[pos1].count;
        }
        size_t pos2 = hf.hash2(key) % size;
        if (table2[pos2].occupied && table2[pos2].key == key) {
            return table2[pos2].count;
        }
        return 0;
    }
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n;
    cin >> n;

    vector<string> student1(n);
    vector<string> student2(n);
    vector<string> student3(n);

    for(int i = 0; i < n; i++) cin >> student1[i];
    for(int i = 0; i < n; i++) cin >> student2[i];
    for(int i = 0; i < n; i++) cin >> student3[i];

    CuckooHashTable set1(400003); // cht(3 * 99999); // 100000
    CuckooHashTable set2(400003);
    CuckooHashTable set3(400003);

    // Добавляем файлы каждого студента в соответствующую таблицу.
    for (int i = 0; i < n; i++) {
        set1.insert(student1[i]);
        set2.insert(student2[i]);
        set3.insert(student3[i]);
    }

    // Переменные для подсчёта итоговых баллов каждого студента.
    long long score1 = 0, score2 = 0, score3 = 0;

    // Подсчитываем баллы для первого студента.
    for (int i = 0; i < n; i++) {
        int cnt = set1.find_count(student1[i]) + // Сколько раз файл первого студента встречается в его таблице.
        set2.find_count(student1[i]) + // Сколько раз этот файл есть у второго студента.
        set3.find_count(student1[i]);  // Сколько раз этот файл есть у третьего студента.
        if (cnt == 1) score1 += 3; // Если файл только у первого студента, добавляем 3 балла.
        else if (cnt == 2) score1 += 1; // Если файл у двух студентов, добавляем 1 балл.
        // Если cnt == 3, баллы не добавляются.
    }

    for (int i = 0; i < n; i++) {
        int cnt = set1.find_count(student2[i]) +
        set2.find_count(student2[i]) +
        set3.find_count(student2[i]);
        if (cnt == 1) score2 += 3;
        else if (cnt == 2) score2 += 1;
    }

    for (int i = 0; i < n; i++) {
        int cnt = set1.find_count(student3[i]) +
        set2.find_count(student3[i]) +
        set3.find_count(student3[i]);
        if (cnt == 1) score3 += 3;
        else if (cnt == 2) score3 += 1;
    }

    cout << score1 << " " << score2 << " " << score3;
    return 0;
}
