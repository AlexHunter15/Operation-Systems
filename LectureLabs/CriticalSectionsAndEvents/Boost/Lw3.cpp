#include <iostream>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <vector>
#include <ctime>
#include <cstdlib>

const int ARRAY_SIZE = 100; // Размер массива
const int NUM_MARKERS = 3;  // Количество потоков marker

boost::mutex mtx; // Мьютекс для доступа к массиву
boost::condition_variable markerCondition[NUM_MARKERS]; // Условные переменные для сигнализации потокам marker
boost::condition_variable mainCondition; // Условная переменная для сигнализации потоку main о продолжении работы

int arr[ARRAY_SIZE] = {0}; // Массив целых чисел

// Функция для потока marker
void MarkerThread(int markerNumber) {
    srand(static_cast<unsigned int>(time(NULL)) + markerNumber);

    while (true) {
        boost::unique_lock<boost::mutex> lock(mtx);
        markerCondition[markerNumber].wait(lock);

        for (int i = 0; i < ARRAY_SIZE; ++i) {
            int randomValue = rand();
            int index = randomValue % ARRAY_SIZE;

            if (arr[index] == 0) {
                boost::this_thread::sleep(boost::posix_time::milliseconds(5));
                arr[index] = markerNumber;
                boost::this_thread::sleep(boost::posix_time::milliseconds(5));
            } else {
                std::cout << "Поток marker " << markerNumber << ": Невозможно пометить элемент " << index << std::endl;
                mainCondition.notify_one();
                markerCondition[markerNumber].wait(lock);
            }
        }

        mainCondition.notify_one();
    }
}

int main() {
    // Запуск потоков marker
    boost::thread markerThreads[NUM_MARKERS];
    for (int i = 0; i < NUM_MARKERS; ++i) {
        markerThreads[i] = boost::thread(MarkerThread, i);
    }

    while (true) {
        boost::unique_lock<boost::mutex> lock(mtx);
        for (int i = 0; i < NUM_MARKERS; ++i) {
            markerCondition[i].notify_one();
        }

        mainCondition.wait(lock);

        // Вывод содержимого массива на консоль
        std::cout << "Содержимое массива:" << std::endl;
        for (int i = 0; i < ARRAY_SIZE; ++i) {
            std::cout << arr[i] << " ";
        }
        std::cout << std::endl;

        int markerToTerminate;
        std::cout << "Введите порядковый номер потока marker для завершения (0-" << NUM_MARKERS - 1 << "): ";
        std::cin >> markerToTerminate;

        markerCondition[markerToTerminate].notify_one();
        mainCondition.wait(lock);

        // Вывод содержимого массива после завершения работы marker
        std::cout << "Содержимое массива после завершения marker " << markerToTerminate << ":" << std::endl;
        for (int i = 0; i < ARRAY_SIZE; ++i) {
            std::cout << arr[i] << " ";
        }
        std::cout << std::endl;
    }

    // Ожидание завершения всех потоков marker
    for (int i = 0; i < NUM_MARKERS; ++i) {
        markerThreads[i].join();
    }

    return 0;
}
