#include "pch.h"
#include "CppUnitTest.h"
#include <Windows.h>
#include <vector>
#include <thread>
#include <chrono>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

// Include the header file with your functions declarations
#include "D:\Alex\Учеба\2 курс\3 семестр\LR3(2)\LR3(2)\LR3(2).cpp"

// Replace "YourHeaderFile.h" with the actual header file name containing your functions declarations

namespace YourTestsNamespace
{
    TEST_CLASS(YourTests)
    {
    public:

        TEST_METHOD(WorkThread_Test)
        {
            // Test data
            std::vector<unsigned int> data = { 1, 2, 3, 4, 5 };
            unsigned int A = 2, B = 4;

            // Expected result
            std::vector<unsigned int> expected_result = { 0, 2, 3, 4, 0 };

            // Initialize resources
            InitializeCriticalSection(&cs_sync);
            event_work_started = CreateEvent(NULL, TRUE, FALSE, NULL);

            // Run the function in a separate thread
            std::thread work_thread(WorkThread, std::cref(data), A, B);
            SetEvent(event_work_started);
            work_thread.join();

            // Check the result
            EnterCriticalSection(&cs_sync);

            for (size_t i = 0; i < expected_result.size(); ++i) {
                Assert::AreEqual(expected_result[i], work_result[i]);
            }

            LeaveCriticalSection(&cs_sync);

            // Clean up resources
            CloseHandle(event_work_started);
            DeleteCriticalSection(&cs_sync);
        }

        TEST_METHOD(MultElementThread_Test)
        {
            // Test data
            std::vector<unsigned int> data = { 1, 2, 3, 4, 5 };
            unsigned int A = 2, B = 4;

            // Expected result
            unsigned long long expected_result = 2 * 3 * 4;

            // Initialize resources
            InitializeCriticalSection(&cs_sync);
            event_work_started = CreateEvent(NULL, TRUE, FALSE, NULL);
            event_mult_element_finished = CreateEvent(NULL, FALSE, FALSE, NULL);

            // Run the function in a separate thread
            std::thread mult_element_thread(MultElementThread, std::cref(data), A, B);
            SetEvent(event_work_started);
            mult_element_thread.join();

            // Wait for the function to complete
            WaitForSingleObject(event_mult_element_finished, INFINITE);

            // Check the result
            Assert::AreEqual(expected_result, mult_result);

            // Clean up resources
            CloseHandle(event_work_started);
            CloseHandle(event_mult_element_finished);
            DeleteCriticalSection(&cs_sync);
        }
    };
}

