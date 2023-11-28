#include <iostream>
#include <windows.h>
using namespace std;

// Объявление массивов для хранения HANDLE'ов событий
HANDLE Parent[3];
HANDLE Child[3];
HANDLE Exit[3];

// Массивы строк для идентификации событий
wchar_t const* ParentMes[3]{
    L"P_Exit",L"P_A",L"P_B"
};
wchar_t const* ChildMes[3]{
    L"Ch_Exit",L"Ch_C",L"Ch_D"
};
wchar_t const* ExitMes[2]{
    L"1",L"0"
};

int main() {
    // Создание событий с помощью функции CreateEvent и сохранение HANDLE'ов в массивах
    for (int i = 0; i < 3; ++i) {
        Parent[i] = CreateEvent(NULL, FALSE, FALSE, ParentMes[i]);
        Child[i] = CreateEvent(NULL, FALSE, FALSE, ChildMes[i]);
    }

    // Создание событий Exit
    for (int i = 0; i < 2; ++i) {
        Exit[i] = CreateEvent(NULL, FALSE, FALSE, ExitMes[i]);
    }

    // Получение числа процессов, которые нужно запустить
    cout << "Input number of process you want to start: ";
    int numberOfProcess;
    cin >> numberOfProcess;

    // Создание семафора с помощью функции CreateSemaphore
    HANDLE hSemaphore = CreateSemaphore(NULL, 2, 2, L"Semaphore");
    HANDLE hMutex = CreateMutex(NULL, FALSE, L"Mutex");


    // Выделение памяти для структур STARTUPINFO и PROCESS_INFORMATION
    STARTUPINFO* Pstp = new STARTUPINFO[numberOfProcess],
        * Chstp = new STARTUPINFO[numberOfProcess];
    PROCESS_INFORMATION* Ppi = new PROCESS_INFORMATION[numberOfProcess],
        * Chpi = new PROCESS_INFORMATION[numberOfProcess];

    // Запуск указанного числа процессов (Parent.exe и Child.exe)
    for (int i = 0; i < numberOfProcess; ++i) {
        ZeroMemory(&Pstp[i], sizeof(STARTUPINFO));
        Pstp[i].cb = sizeof(STARTUPINFO);
        ZeroMemory(&Chstp[i], sizeof(STARTUPINFO));
        Chstp[i].cb = sizeof(STARTUPINFO);
        wstring parent = L"Parent.exe",
            child = L"Child.exe";
        CreateProcess(NULL, &parent[0], NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &Pstp[i], &Ppi[i]);
        CreateProcess(NULL, &child[0], NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &Chstp[i], &Chpi[i]);
    }

    // Цикл для обработки событий и завершения процессов
    int counter = 1;
    while (true) {
        // Ожидание событий от родительских процессов и дочерних процессов
        DWORD mes = WaitForMultipleObjects(6, Parent, FALSE, INFINITE);

        // Обработка событий от родительских процессов
        if (mes >= WAIT_OBJECT_0 && mes <= WAIT_OBJECT_0 + 2) {
            //SetEvent(Child[mes]); // Установка события для дочернего процесса
            if (mes == WAIT_OBJECT_0) {
                // Если получено событие P_Exit, ожидание события из массива Exit
                DWORD mes1 = WaitForMultipleObjects(2, Exit, FALSE, INFINITE);
                if (mes1 == WAIT_OBJECT_0) {
                    cout << "Child " << counter << " ended his work\n";
                    counter += 1;
                }
                break; // Завершение цикла после получения события P_Exit
            }
            wcout << L"Received from Parent: " << ParentMes[mes - WAIT_OBJECT_0] + 2 << L'\n';
        }

        // Обработка событий от дочерних процессов
        if (mes >= WAIT_OBJECT_0 + 3 && mes <= WAIT_OBJECT_0 + 5) {
            wcout << L"Received from Child: " << ChildMes[mes - WAIT_OBJECT_0 - 3] + 2 << L'\n';
            // Дополнительная обработка сообщений от дочерних процессов
            // Можно добавить нужную логику обработки событий от дочерних процессов здесь
        }
    }


    // Закрытие HANDLE'ов для событий Parent и Child
    for (int i = 0; i < 3; ++i) {
        CloseHandle(Parent[i]);
        CloseHandle(Child[i]);
    }

    // Закрытие HANDLE'ов для событий Exit
    for (int i = 0; i < 2; ++i) {
        CloseHandle(Exit[i]);
    }

    // Закрытие HANDLE'ов для процессов и потоков
    for (int i = 0; i < numberOfProcess; ++i) {
        CloseHandle(Ppi[i].hThread);
        CloseHandle(Ppi[i].hProcess);
        CloseHandle(Chpi[i].hThread);
        CloseHandle(Chpi[i].hProcess);
    }

    // Закрытие семафора
    CloseHandle(hSemaphore);
    return 0;
}
