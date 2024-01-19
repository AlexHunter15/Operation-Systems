#include "pch.h"
#include "CppUnitTest.h"
#include "D:\Alex\Учеба\2 курс\3 семестр\LR7\LR7\LR7.cpp"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;



namespace YourTestsNamespace
{
    TEST_CLASS(YourTests)
    {
    public:

        TEST_METHOD(CreateThreadsTest)
        {
            HANDLE RWThreads[] =
            {
                CreateThread(NULL, 0, runReaders, nullptr, 0, 0),
                CreateThread(NULL, 0, runWriters, nullptr, 0, 0)
            };

            Assert::IsNotNull(RWThreads[0]); // Check if the reader thread is created
            Assert::IsNotNull(RWThreads[1]); // Check if the writer thread is created

            WaitForMultipleObjects(2, RWThreads, true, INFINITE); // Wait for threads to finish

            CloseHandle(RWThreads[0]);
            CloseHandle(RWThreads[1]);
        }

        TEST_METHOD(MutexAndEventTest)
        {
            HANDLE hSignalEvent = CreateEvent(NULL, FALSE, TRUE, _T("SignalEvent"));
            HANDLE hWriterMutex = CreateMutex(NULL, FALSE, _T("WriterMutex"));
            HANDLE hReaderMutex = CreateMutex(NULL, FALSE, _T("ReaderMutex"));

            Assert::IsNotNull(hSignalEvent); // Check if the event is created
            Assert::IsNotNull(hWriterMutex); // Check if the writer mutex is created
            Assert::IsNotNull(hReaderMutex); // Check if the reader mutex is created

            SetEvent(hSignalEvent); // Set the event

            DWORD waitResult = WaitForSingleObject(hWriterMutex, 0);
            Assert::AreEqual(WAIT_OBJECT_0, waitResult); // Check if we can successfully acquire the writer mutex

            waitResult = WaitForSingleObject(hReaderMutex, 0);
            Assert::AreEqual(WAIT_OBJECT_0, waitResult); // Check if we can successfully acquire the reader mutex

            ReleaseMutex(hWriterMutex); // Release the writer mutex
            ReleaseMutex(hReaderMutex); // Release the reader mutex

            CloseHandle(hSignalEvent);
            CloseHandle(hWriterMutex);
            CloseHandle(hReaderMutex);
        }

        // Add more tests as needed based on the specific functionality you want to test
    };
}