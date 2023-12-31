#include <iostream>
#include <vector>
#include <windows.h>
#include <process.h>

std::vector<int> arr;           
int min_element, max_element;   
double average;                

void FindMinMax(void* param) {
    min_element = arr[0];
    max_element = arr[0];

    for (int i = 1; i < arr.size(); ++i) {
        if (arr[i] < min_element) {
            min_element = arr[i];
        }

        if (arr[i] > max_element) {
            max_element = arr[i];
        }

        Sleep(7); 
    }

    std::cout << "Минимальный элемент: " << min_element << std::endl;
    std::cout << "Максимальный элемент: " << max_element << std::endl;
}

void CalculateAverage(void* param) {
    int sum = 0;

    for (int i = 0; i < arr.size(); ++i) {
        sum += arr[i];
        Sleep(12); 
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

    _beginthread(FindMinMax, 0, NULL);
    _beginthread(CalculateAverage, 0, NULL);

    Sleep(150); 

    for (int i = 0; i < arr.size(); ++i) {
        if (arr[i] == min_element || arr[i] == max_element) {
            arr[i] = static_cast<int>(average);
        }
    }

    std::cout << "Массив после замены:" << std::endl;
    for (int i = 0; i < arr.size(); ++i) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}
