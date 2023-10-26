#include <iostream>
#include <vector>
#include <thread>
#include <Windows.h>

CRITICAL_SECTION cs_sync;
HANDLE work_thread_handle;
HANDLE mult_element_thread_handle;
HANDLE event_work_started;
HANDLE event_mult_element_finished;
unsigned long long mult_result = 1;
std::vector<unsigned int> work_result;

void WorkThread(const std::vector<unsigned int>& data, unsigned int A, unsigned int B) {
    EnterCriticalSection(&cs_sync);
    WaitForSingleObject(event_work_started, INFINITE);
    std::vector<unsigned int> result(data.size(), 0);

    for (size_t i = 0; i < data.size(); ++i) {
        if (data[i] >= A && data[i] <= B) {
            result[i] = data[i];
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

   


    work_result = result;
    LeaveCriticalSection(&cs_sync);
}

void MultElementThread(const std::vector<unsigned int>& data, unsigned int A, unsigned int B) {
    WaitForSingleObject(event_work_started, INFINITE);
    for (unsigned int num : data) {
        if (num >= A && num <= B) {
            mult_result *= num;
        }
    }

    SetEvent(event_mult_element_finished);
}

int main() {
    setlocale(LC_ALL, "rus");
    std::vector<unsigned int> data;
    unsigned int A, B;
    std::cout << "Введите размерность массива: ";
    size_t size;
    std::cin >> size;

    std::cout << "Введите элементы массива:" << std::endl;
    for (size_t i = 0; i < size; ++i) {
        unsigned int num;
        std::cin >> num;
        data.push_back(num);
    }

  

    InitializeCriticalSection(&cs_sync);

    event_work_started = CreateEvent(NULL, TRUE, FALSE, NULL);

    event_mult_element_finished = CreateEvent(NULL, FALSE, FALSE, NULL);

    std::cout << "Введите значения A и B: ";
    std::cin >> A >> B;

    std::thread work_thread(WorkThread, std::cref(data), A, B);
    std::thread mult_element_thread(MultElementThread, std::cref(data), A, B);
   

    SetEvent(event_work_started);

    work_thread.join();

    EnterCriticalSection(&cs_sync);

    std::cout << "Work Thread: ";
    for (unsigned int num : work_result) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    LeaveCriticalSection(&cs_sync);

    WaitForSingleObject(event_mult_element_finished, INFINITE);

    std::cout << "MultElement Result: " << mult_result << std::endl;

    
    mult_element_thread.join();
    
    CloseHandle(event_work_started);
    CloseHandle(event_mult_element_finished);

    DeleteCriticalSection(&cs_sync);

    return 0;
}

/*work_thread_handle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)WorkThread, (LPVOID)&data, 0, NULL);
mult_element_thread_handle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)MultElementThread, (LPVOID)&data, 0, NULL);

WaitForSingleObject(work_thread_handle, INFINITE);
WaitForSingleObject(mult_element_thread_handle, INFINITE);
*/