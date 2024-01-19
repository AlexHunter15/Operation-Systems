#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

// Include the header file with your functions declarations
#include "D:\Alex\Учеба\2 курс\3 семестр\LR6\LR\LR.cpp"


namespace YourTestsNamespace
{
    TEST_CLASS(YourTests)
    {
    public:

        TEST_METHOD(MultElementFunctionTest)
        {
            std::vector<unsigned int> testArray = { 1, 2, 3, 4, 5 };
            unsigned int A = 2, B = 4;

            std::thread multElementThread(multElement, std::ref(testArray), A, B);

            // Allow some time for the thread to start
            std::this_thread::sleep_for(std::chrono::milliseconds(100));

            {
                std::lock_guard<std::mutex> lock(mtx);
                readyWork = true;
                cvWork.notify_all();
            }

            multElementThread.join();

            // Check the result
            unsigned long long expectedMultElementResult = 2 * 3 * 4;
            Assert::AreEqual(expectedMultElementResult, multElementResult);
        }
    };
}