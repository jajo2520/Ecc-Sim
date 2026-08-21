#pragma once
#include <string>
#include <iostream>

namespace test
{
    inline unsigned int testsPassed {};
    inline unsigned int testsFailed {};


    template <typename T>
    void checkEquality(T& actual, T& expected, const std::string& testName)
    {
        if (actual != expected)
        {
            std::cout << "FAIL:" << testName << ", expected: " << expected << ", got: actual" << std::endl;
            ++testsFailed;
        }
        else 
            ++testsPassed;
   }

    inline void checkTrue(bool condition, const std::string& testName)
    {
        if (!condition)
        {
            std::cout << "FAIL:" << testName << ", expected: true, got: false" << std::endl;
            ++testsFailed;
        }
        else 
            ++testsPassed;
    }

    inline void printTotals()
    {
        std::cout << "Total tests: " << testsPassed+testsFailed << std::endl;
        std::cout << "Tests passed: " << testsPassed << "\n Tests failed: " << testsFailed << std::endl;
    }
}
