#include <iostream>
#include <Windows.h>
#include <process.h>

const int MAX_ARRAY_SIZE = 1000;

struct WorkerThreadParams {
    int* array;
    int arraySize;
    int searchValue;
    int* result;
};

struct CountThreadParams {
    int* result;
};

unsigned __stdcall WorkerThread(void* params) {
    WorkerThreadParams* threadParams = static_cast<WorkerThreadParams*>(params);
    Sleep(200);

    *(threadParams->result) = 0;
    for (int i = 0; i < threadParams->arraySize; ++i) {
        if (threadParams->array[i] > threadParams->searchValue) {
            (*(threadParams->result))++;
        }
    }

    return 0;
}

unsigned __stdcall CountThread(void* params) {
    CountThreadParams* threadParams = static_cast<CountThreadParams*>(params);

    int a = 0, b = 1;
    while (true) {
        std::cout << a << " ";
        int temp = a;
        a = b;
        b += temp;
        Sleep(1000);
    }

    return 0;
}

int main() {
    std::cout << "Enter the size of the array: ";
    int arraySize;
    std::cin >> arraySize;

    if (arraySize > MAX_ARRAY_SIZE) {
        std::cerr << "Array size exceeds the maximum limit." << std::endl;
        return 1;
    }

    int array[MAX_ARRAY_SIZE];
    std::cout << "Enter the elements of the array: ";
    for (int i = 0; i < arraySize; ++i) {
        std::cin >> array[i];
    }

    int searchValue;
    std::cout << "Enter the search value: ";
    std::cin >> searchValue;

    int result = 0;

    WorkerThreadParams workerParams;
    workerParams.array = array;
    workerParams.arraySize = arraySize;
    workerParams.searchValue = searchValue;
    workerParams.result = &result;

    HANDLE hThreadWorker = (HANDLE)_beginthreadex(nullptr, 0, WorkerThread, &workerParams, 0, nullptr);

    int startTime, stopTime;
    std::cout << "Enter the start time (in seconds): ";
    std::cin >> startTime;
    std::cout << "Enter the stop time (in seconds): ";
    std::cin >> stopTime;

    CountThreadParams countParams;
    countParams.result = &result;

    HANDLE hThreadCount = (HANDLE)_beginthreadex(nullptr, 0, CountThread, &countParams, CREATE_SUSPENDED, nullptr);
    ResumeThread(hThreadCount);
    Sleep(startTime * 1000);

    ResumeThread(hThreadWorker);

    WaitForSingleObject(hThreadWorker, 2);

    TerminateThread(hThreadCount, 0);
    CloseHandle(hThreadCount);

    std::cout << "Result: " << result << " elements are greater than " << searchValue << std::endl;

    CloseHandle(hThreadWorker);
    return 0;
}