#include "pch.h"
#include "CppUnitTest.h"
#include <Windows.h>
#include <process.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "D:\Alex\Учеба\2 курс\3 семестр\LR1(oc)\LR1(oc)\LR1(oc).cpp"

TEST_CLASS(MyTests)
{
public:

    // Test for WorkerThread
    TEST_METHOD(WorkerThread_Test)
    {
        int array[] = { 1, 3, 5, 7, 9 };
        int arraySize = sizeof(array) / sizeof(array[0]);
        int searchValue = 4;
        int result = 0;

        WorkerThreadParams workerParams;
        workerParams.array = array;
        workerParams.arraySize = arraySize;
        workerParams.searchValue = searchValue;
        workerParams.result = &result;

        WorkerThread(&workerParams);

        Assert::AreEqual(3, result); // Expecting 3 elements greater than 4
    }

    // Test for CountThread
    TEST_METHOD(CountThread_Test)
    {
        int result = 0;

        CountThreadParams countParams;
        countParams.result = &result;

        HANDLE hThreadCount = (HANDLE)_beginthreadex(nullptr, 0, CountThread, &countParams, CREATE_SUSPENDED, nullptr);

        ResumeThread(hThreadCount);

        Sleep(2000); // Sleep for 2 seconds

        TerminateThread(hThreadCount, 0);
        CloseHandle(hThreadCount);

        Assert::AreEqual(0, result);
    }
};
