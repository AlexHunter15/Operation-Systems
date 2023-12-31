#include <iostream>
#include <vector>
#include <boost/thread.hpp>

std::vector<int> arr;
int min_element, max_element;
double average;

void FindMinMax() {
    min_element = arr[0];
    max_element = arr[0];

    for (int i = 1; i < arr.size(); ++i) {
        if (arr[i] < min_element) {
            min_element = arr[i];
        }

        if (arr[i] > max_element) {
            max_element = arr[i];
        }

        boost::this_thread::sleep(boost::posix_time::milliseconds(7));
    }

    std::cout << "Минимальный элемент: " << min_element << std::endl;
    std::cout << "Максимальный элемент: " << max_element << std::endl;
}

void CalculateAverage() {
    int sum = 0;

    for (int i = 0; i < arr.size(); ++i) {
        sum += arr[i];
        boost::this_thread::sleep(boost::posix_time::milliseconds(12));
    }

    average = static_cast<double>(sum) / arr.size();
    std::cout << "Среднее значение элементов: " << average << std::endl;
}

int main() {
    int n;
    std::cout << "Введите размерность массива: ";
    std::cin >> n;

    std::cout << "Введите элементы массива:" << std::endl;
    for (int i = 0; i < n; ++i) {
        int element;
        std::cin >> element;
        arr.push_back(element);
    }

    boost::thread minMaxThread(FindMinMax);
    boost::thread averageThread(CalculateAverage);

    minMaxThread.join();
    averageThread.join();

    for (int& element : arr) {
        if (element == min_element || element == max_element) {
            element = static_cast<int>(average);
        }
    }

    std::cout << "Массив после замены:" << std::endl;
    for (int element : arr) {
        std::cout << element << " ";
    }
    std::cout << std::endl;

    return 0;
}
