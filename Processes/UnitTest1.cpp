#include "pch.h"
#include "CppUnitTest.h"

#include <sstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

// Include the header file with your functions declarations
#include "D:\Alex\Учеба\2 курс\3 семестр\LR2\LR2\LR2.cpp"

// Replace "YourHeaderFile.h" with the actual header file name containing your functions declarations

namespace YourTestsNamespace
{
    TEST_CLASS(YourTests)
    {
    public:

        // Test for the IsPrime function
        TEST_METHOD(IsPrimeTest)
        {
            Assert::IsTrue(IsPrime(2));  // 2 is a prime number
            Assert::IsTrue(IsPrime(13)); // 13 is a prime number
            Assert::IsTrue(IsPrime(17)); // 17 is a prime number
            Assert::IsFalse(IsPrime(1)); // 1 is not a prime number
            Assert::IsFalse(IsPrime(4)); // 4 is not a prime number
            Assert::IsFalse(IsPrime(15)); // 15 is not a prime number
        }

      
    };
}