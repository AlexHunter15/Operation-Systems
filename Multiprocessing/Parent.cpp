#include <iostream>
#include <windows.h>
using namespace std;

// Массив строк для идентификации событий
wchar_t const* mes[3]{
    L"P_Exit", L"P_A", L"P_B"
};

int main() {
    // Открытие семафора для контроля доступа к ресурсам
    HANDLE hSemaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, L"Semaphore");
    WaitForSingleObject(hSemaphore, INFINITE); // Ожидание доступа к семафору

    // Вывод информации о запуске родительского процесса
    cout << "Process parent is started" << endl;
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

    // Освобождение семафора, чтобы разрешить доступ другим процессам
    ReleaseSemaphore(hSemaphore, 1, NULL);

    return 0;
}
