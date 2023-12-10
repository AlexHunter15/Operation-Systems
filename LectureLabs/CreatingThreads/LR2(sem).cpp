#include <iostream>
#include <Windows.h>
#include <vector>
#include <thread>

// Глобальные переменные
std::vector<int> arr;            // Массив целых чисел
int min_element, max_element;   // Минимальный и максимальный элементы
double average;                // Среднее значение элементов

// Функция для потока min_max
void FindMinMax() {
    setlocale(LC_ALL, "rus");
    min_element = arr[0];
    max_element = arr[0];

    for (int i = 1; i < arr.size(); ++i) {
        if (arr[i] < min_element) {
            min_element = arr[i];
        }

        if (arr[i] > max_element) {
            max_element = arr[i];
        }

        Sleep(7); // Приостановить выполнение на 7 миллисекунд
    }

    std::cout << "Минимальный элемент: " << min_element << std::endl;
    std::cout << "Максимальный элемент: " << max_element << std::endl;
}

// Функция для потока average
void CalculateAverage() {
    setlocale(LC_ALL, "rus");
    int sum = 0;

    for (int i = 0; i < arr.size(); ++i) {
        sum += arr[i];
        Sleep(12); // Приостановить выполнение на 12 миллисекунд
    }

    average = static_cast<double>(sum) / arr.size();
    std::cout << "Среднее значение элементов: " << average << std::endl;
}

int main() {
    setlocale(LC_ALL, "rus");
    // Ввод размерности и элементов массива
    int n;
    std::cout << "Введите размерность массива: ";
    std::cin >> n;

    std::cout << "Введите элементы массива:" << std::endl;
    for (int i = 0; i < n; ++i) {
        int element;
        std::cin >> element;
        arr.push_back(element);
    }

    // Создание потоков min_max и average
    std::thread minMaxThread(FindMinMax);
    std::thread averageThread(CalculateAverage);

    // Ожидание завершения потоков min_max и average
    minMaxThread.join();
    averageThread.join();

    // Замена максимального и минимального элементов на среднее значение
    for (int& element : arr) {
        if (element == min_element || element == max_element) {
            element = static_cast<int>(average);
        }
    }

    // Вывод полученных результатов
    std::cout << "Массив после замены:" << std::endl;
    for (int element : arr) {
        std::cout << element << " ";
    }
    std::cout << std::endl;

    return 0;
}
