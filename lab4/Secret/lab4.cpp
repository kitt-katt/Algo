#include <bits/stdc++.h>
using namespace std;

// Проверяем, что часть имени соответствует требованиям
bool checkNamePart(const string &s) {
    if (s.empty()) return false;
    if (!isalpha((unsigned char)s[0]) || !isupper((unsigned char)s[0])) return false; // Первая буква должна быть заглавной
    for (int i = 1; i < (int)s.size(); i++) {
        if (!isalpha((unsigned char)s[i]) || !islower((unsigned char)s[i])) return false; // Остальные буквы должны быть строчными
    }
    return true;
}

// Проверяем ФИО, состоящее из трёх частей
bool checkFIO(const vector<string> &parts) {
    if (parts.size() != 3) return false; // Должно быть ровно три слова
    for (auto &p : parts) {
        if (!checkNamePart(p)) return false; // Каждая часть должна соответствовать правилам
    }
    return true;
}

// Проверяем корректность возраста
bool checkAge(const string &age) {
    if (age.empty()) return false;
    for (auto c : age) {
        if (!isdigit((unsigned char)c)) return false; // Возраст должен состоять только из цифр
    }
    return true;
}

// Проверяем корректность номера телефона
bool checkPhone(const string &phone) {
    string digits;
    for (auto c : phone) {
        if (isdigit((unsigned char)c)) digits.push_back(c); // Извлекаем все цифры
    }

    if (digits.size() != 11) return false; // Номер должен содержать ровно 11 цифр

    // Проверяем первый символ после удаления лишних символов
    if (!phone.empty() && phone[0] == '+') {
        if (digits[0] != '7') return false; // Если номер начинается с "+", первая цифра должна быть 7
    } else {
        if (digits[0] != '7' && digits[0] != '8') return false; // Без "+", допустимы 7 или 8
    }

    return true;
}

// Проверяем корректность адреса
bool checkAddress(const string &addr) {
    vector<string> parts;
    {
        size_t start = 0;
        while (true) {
            size_t pos = addr.find(", ", start); // Разделяем строку по запятым с пробелами
            if (pos == string::npos) {
                parts.push_back(addr.substr(start));
                break;
            } else {
                parts.push_back(addr.substr(start, pos - start));
                start = pos + 2;
            }
        }
    }

    if (parts.size() != 3) return false; // Адрес должен состоять из трех частей

    // Проверяем часть города
    {
        const string &gpart = parts[0];
        if (gpart.size() != 5 || gpart[0] != 'g' || gpart[1] != '.') return false;
        for (int i = 2; i < 5; i++) {
            if (!isalpha((unsigned char)gpart[i]) || !isupper((unsigned char)gpart[i])) return false;
        }
    }

    // Проверяем часть улицы
    {
        const string &ulpart = parts[1];
        if (ulpart.size() < 4 || ulpart.substr(0, 3) != "ul.") return false;
        string street = ulpart.substr(3);
        if (!isupper((unsigned char)street[0])) return false;
        for (int i = 1; i < (int)street.size(); i++) {
            if (!isalpha((unsigned char)street[i]) || !islower((unsigned char)street[i]))
                return false;
        }
    }

    // Проверяем номер дома
    {
        const string &dpart = parts[2];
        if (dpart.size() < 4 || dpart.substr(0, 2) != "d.") return false;
        string house = dpart.substr(2);
        if (house.size() < 2) return false;
        for (auto c : house) {
            if (!isdigit((unsigned char)c)) return false;
        }
    }

    return true;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int M; 
    cin >> M;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    for (int i = 0; i < M; i++) {
        string line;
        getline(cin, line); // Читаем строку данных жильца

        vector<string> tokens;
        {
            stringstream ss(line);
            string t;
            while (ss >> t) tokens.push_back(t); // Разделяем строку на токены
        }

        if (tokens.size() < 5) {
            cout << "NO\n";
            continue;
        }

        vector<string> fio = {tokens[0], tokens[1], tokens[2]};
        string age = tokens[3];
        string phone = tokens[4];

        string addr;
        {
            for (int j = 5; j < (int)tokens.size(); j++) {
                if (!addr.empty()) addr += " ";
                addr += tokens[j];
            }
        }

        bool valid = true;
        if (!checkFIO(fio)) valid = false;
        if (!checkAge(age)) valid = false;
        if (!checkPhone(phone)) valid = false;
        if (!checkAddress(addr)) valid = false;

        cout << (valid ? "YES" : "NO") << "\n"; // Вывод результата для каждого жильца
    }

    return 0;
}
