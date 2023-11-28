#include <iostream>
#include <string>
#include <windows.h>

using namespace std;

// Массив строк для идентификации событий
wchar_t const* mes[3]{
    L"Ch_Exit", L"Ch_C", L"Ch_D"
};

// Массив строк для идентификации событий завершения
wchar_t const* EMes[2]{
    L"1", L"0"
};

int main() {
    // Открытие мьютекса для контроля доступа к ресурсам
    HANDLE hMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, L"Mutex");
    WaitForSingleObject(hMutex, INFINITE); // Ожидание доступа к мьютексу

    // Вывод информации о запуске родительского процесса
    cout << "Process child is started" << endl;
    cout << "Process is active" << endl;

    // Создание массива для HANDLE'ов событий
    HANDLE mesEv[3];

    // Открытие событий с помощью OpenEvent и сохранение HANDLE'ов в массиве mesEv
    for (int i = 0; i < 3; ++i)
        mesEv[i] = OpenEvent(EVENT_MODIFY_STATE, FALSE, mes[i]);

    // Основной цикл родительского процесса
    while (true) {
        string line;
        cin >> line;

        // Формирование строки для сравнения с событиями
        line = "P_" + line;
        wstring wline(line.begin(), line.end());

        // Проверка введенной строки и установка соответствующего события
        if (wline._Equal(mes[0])) {
            SetEvent(mesEv[0]); // Установка события P_Exit
            break; // Завершение цикла
        }
        if (wline._Equal(mes[1])) {
            SetEvent(mesEv[1]); // Установка события P_A
        }
        if (wline._Equal(mes[2])) {
            SetEvent(mesEv[2]); // Установка события P_B
        }
    }

    // Освобождение мьютекса, чтобы разрешить доступ другим процессам
    ReleaseMutex(hMutex);

    return 0;
}
