#include <iostream>
#include <windows.h>
#include <vector>

using namespace std;

bool isPrime(int num) {
    if (num <= 1) return false;
    for (int i = 2; i * i <= num; ++i) {
        if (num % i == 0) return false;
    }
    return true;
}

int main(int argc, char* argv[]) {
    HANDLE hWritePipe = (HANDLE)atoi(argv[1]), hReadPipe = (HANDLE)atoi(argv[2]);

    int size;
    DWORD dwBytesRead;
    ReadFile(hReadPipe, &size, sizeof(int), &dwBytesRead, NULL);
    vector<int> arr(size);
    cout << "Generated array"<<endl;
    for (int i = 0; i < size; i++) {
        arr[i] = rand() % 100;
        cout << arr[i] << endl;
    }
    cout << "Primes array"<<endl;
    vector<int> primes;
    for (int i = 0; i < size; ++i) {
        if (isPrime(arr[i])) {
            primes.push_back(arr[i]);
            cout << arr[i] << endl;
        }
    }

    int primesCount = primes.size();
    WriteFile(hWritePipe, &primesCount, sizeof(int), &dwBytesRead, NULL);

    for (int i = 0; i < primesCount; ++i) {
        WriteFile(hWritePipe, &primes[i], sizeof(int), &dwBytesRead, NULL);
    }
    CloseHandle(hWritePipe);
    CloseHandle(hReadPipe);
    int a;
    cin >> a;
    return 0;
}
