#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <random>
#include <chrono>

std::mutex mtx;
std::condition_variable cvWork, cvMultElement;
bool readyWork = false, readyMultElement = false;

void work(const std::vector<unsigned int>& arr, unsigned int A, unsigned int B) {
    std::unique_lock<std::mutex> lock(mtx);
    cvWork.wait(lock, [] { return readyWork; });

    int restInterval;
    std::cout << "Введите временной интервал для отдыха (в миллисекундах): ";
    std::cin >> restInterval;

    std::vector<unsigned int> result(arr.size(), 0);
    for (size_t i = 0; i < arr.size(); ++i) {
        if (arr[i] >= A && arr[i] <= B) {
            result[i] = arr[i];
            std::this_thread::sleep_for(std::chrono::milliseconds(restInterval));
        }
    }

    std::cout << "Результат работы потока Work:" << std::endl;
    for (const auto& val : result) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    readyWork = true;
    cvMultElement.notify_one();
}

void multElement(const std::vector<unsigned int>& arr, unsigned int A, unsigned int B) {
    std::unique_lock<std::mutex> lock(mtx);
    cvWork.wait(lock, [] { return readyWork; });

    unsigned long long result = 1;
    for (const auto& val : arr) {
        if (val >= A && val <= B) {
            result *= val;
        }
    }

    std::cout << "Результат работы потока MultElement: " << result << std::endl;

    readyMultElement = true;
    cvMultElement.notify_one();
}

int main() {
    setlocale(LC_ALL, "rus");
    size_t size;
    std::cout << "Введите размерность массива: ";
    std::cin >> size;

    std::vector<unsigned int> array(size);
    std::cout << "Выберите способ заполнения массива (1 - вручную, 2 - случайно): ";
    int choice;
    std::cin >> choice;

    if (choice == 1) {
        std::cout << "Введите элементы массива:" << std::endl;
        for (size_t i = 0; i < size; ++i) {
            std::cin >> array[i];
        }
    }
    else {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<unsigned int> dis(1, 100);

        for (size_t i = 0; i < size; ++i) {
            array[i] = dis(gen);
        }
    }

    std::cout << "Размерность массива и его элементы:" << std::endl;
    for (const auto& val : array) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    unsigned int A, B;
    std::cout << "Введите значения A и B: ";
    std::cin >> A >> B;

    std::thread tWork(work, std::ref(array), A, B);
    std::thread tMultElement(multElement, std::ref(array), A, B);

    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Даем потокам время для запуска

    {
        std::lock_guard<std::mutex> lock(mtx);
        readyWork = true;
        cvWork.notify_all();
    }

    {
        std::unique_lock<std::mutex> lock(mtx);
        cvMultElement.wait(lock, [] { return readyMultElement; });
    }

    tWork.join();
    tMultElement.join();

    return 0;
}
