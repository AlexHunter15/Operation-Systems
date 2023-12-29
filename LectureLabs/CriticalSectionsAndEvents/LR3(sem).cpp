#include <iostream>
#include <windows.h>
#include <vector>
#include <thread>
#include <ctime>
#include <cstdlib>

const int ARRAY_SIZE = 100; // Размер массива
const int NUM_MARKERS = 3;  // Количество потоков marker

CRITICAL_SECTION cs;        // Критическая секция для доступа к массиву
HANDLE markers[NUM_MARKERS]; // События для сигнализации потокам marker
HANDLE mainContinueEvent;    // Событие для сигнализации потоку main о продолжении работы

std::vector<int> arr(ARRAY_SIZE, 0); // Массив целых чисел

// Функция для потока marker
void MarkerThread(int markerNumber) {
    setlocale(LC_ALL, "rus");
    srand(static_cast<unsigned int>(time(nullptr)) + markerNumber);

    while (true) {
        // Ждем сигнала от main для начала работы
        WaitForSingleObject(markers[markerNumber], INFINITE);

        for (int i = 0; i < ARRAY_SIZE; ++i) {
            int randomValue = rand();
            int index = randomValue % ARRAY_SIZE;

            EnterCriticalSection(&cs);

            if (arr[index] == 0) {
                Sleep(5); // Поспать 5 миллисекунд
                arr[index] = markerNumber;
                Sleep(5); // Поспать еще 5 миллисекунд
            }
            else {
                // Сигнализировать main о невозможности продолжения работы
                std::cout << "Поток marker " << markerNumber << ": Невозможно пометить элемент " << index << std::endl;
                SetEvent(mainContinueEvent);

                // Ждем ответный сигнал от main
                WaitForSingleObject(markers[markerNumber], INFINITE);
            }

            LeaveCriticalSection(&cs);
        }

        // Сигнализировать main о завершении работы
        SetEvent(mainContinueEvent);
    }
}

int main() {
    setlocale(LC_ALL, "rus");
    InitializeCriticalSection(&cs);

    // Создаем события для потоков marker и main
    for (int i = 0; i < NUM_MARKERS; ++i) {
        markers[i] = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    }
    mainContinueEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);

    // Ввод размерности массива
    int n;
    std::cout << "Введите размерность массива: ";
    std::cin >> n;

    // Запуск потоков marker
    std::vector<std::thread> markerThreads;
    for (int i = 0; i < NUM_MARKERS; ++i) {
        markerThreads.push_back(std::thread(MarkerThread, i));
    }

    // Дать сигнал на начало работы всех потоков marker
    for (int i = 0; i < NUM_MARKERS; ++i) {
        SetEvent(markers[i]);
    }

    while (true) {
        // Ждем, пока все потоки marker не подадут сигналы о невозможности продолжения работы
        WaitForMultipleObjects(NUM_MARKERS, markers, TRUE, INFINITE);

        // Вывод содержимого массива на консоль
        std::cout << "Содержимое массива:" << std::endl;
        for (int i = 0; i < n; ++i) {
            std::cout << arr[i] << " ";
        }
        std::cout << std::endl;

        // Запрос порядкового номера потока marker для завершения
        int markerToTerminate;
        std::cout << "Введите порядковый номер потока marker для завершения (0-" << NUM_MARKERS - 1 << "): ";
        std::cin >> markerToTerminate;

        // Подать сигнал на завершение работы выбранному потоку marker
        SetEvent(markers[markerToTerminate]);

        // Ждем завершения работы выбранного потока marker
        WaitForSingleObject(mainContinueEvent, INFINITE);

        // Вывод содержимого массива после завершения работы marker
        std::cout << "Содержимое массива после завершения marker " << markerToTerminate << ":" << std::endl;
        for (int i = 0; i < n; ++i) {
            std::cout << arr[i] << " ";
        }
        std::cout << std::endl;

        // Подаем сигнал на продолжение работы оставшимся потокам marker
        for (int i = 0; i < NUM_MARKERS; ++i) {
            ResetEvent(mainContinueEvent);
            SetEvent(markers[i]);
        }
    }

    // Ожидание завершения всех потоков marker
    for (int i = 0; i < NUM_MARKERS; ++i) {
        WaitForSingleObject(markerThreads[i].native_handle(), INFINITE);
        markerThreads[i].join();
    }

    // Удаление событий и критической секции
    for (int i = 0; i < NUM_MARKERS; ++i) {
        CloseHandle(markers[i]);
    }
    CloseHandle(mainContinueEvent);
    DeleteCriticalSection(&cs);

    return 0;
}
