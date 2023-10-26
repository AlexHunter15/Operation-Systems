#include <iostream>
#include <vector>
#include <sstream>
#include <windows.h>

bool IsPrime(int num) {
    if (num <= 1) return false;
    if (num <= 3) return true;
    if (num % 2 == 0 || num % 3 == 0) return false;
    for (int i = 5; i * i <= num; i += 6) {
        if (num % i == 0 || num % (i + 2) == 0) return false;
    }
    return true;
}

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "Russian");

    if (argc < 2) {
        std::cerr << "Необходимо передать количество элементов и элементы массива через аргументы командной строки." << std::endl;
        return 1;
    }

    int n;
    std::istringstream(argv[1]) >> n;

    std::vector<int> arr(n);

    for (int i = 0; i < n; ++i) {
        std::istringstream(argv[i + 2]) >> arr[i];
    }

    std::vector<int> primes;
    std::vector<int> nonPrimes;

    for (int i = 0; i < n; ++i) {
        if (IsPrime(arr[i])) {
            primes.push_back(arr[i]);
        } else {
            nonPrimes.push_back(arr[i]);
        }
    }

    std::cout << "Простые числа: ";
    for (int prime : primes) {
        std::cout << prime << " ";
    }
    std::cout << std::endl;

    std::cout << "Непростые числа: ";
    for (int nonPrime : nonPrimes) {
        std::cout << nonPrime << " ";
    }
    std::cout << std::endl;
    int a;
    std::cin >> a;
    return 0;
}

