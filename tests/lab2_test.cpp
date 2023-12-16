#include <gtest/gtest.h>

#include "lab2.hpp"
#include "utils.hpp"

#include <chrono>
#include <limits>

namespace {
    TMatrix GenerateMatrix(int m, int n) {
        TMatrix result(m, std::vector<std::complex<int>>(n));

        std::srand(std::time(nullptr));

        for(int i = 0; i < m; ++i) {
            for(int j = 0; j < n; ++j) {
                result[i][j] = std::rand() % 100;
            }
        }

        return result;
    }
}

bool AreEqual(TMatrix lhs, TMatrix rhs) {
    if(lhs.size() != rhs.size() || lhs[0].size() != rhs[0].size()) {
        return false;
    }

    for(int i = 0; i < isize(lhs); ++i) {
        if(lhs[i].size() != rhs[i].size()) {
            return false;
        }

        for(int j = 0; j < isize(lhs); ++j) {
            if(lhs[i][j] != rhs[i][j]) {
                return false;
            }
        }
    }
    return true;
}

TEST(SecondLab, SingleThreadWithCorrectResults1) {
    ASSERT_TRUE(AreEqual(MultiplyMatrices(
    TMatrix{
        {std::complex<int>(2, 3), std::complex<int>(1, 4)},
        {std::complex<int>(7, 1), std::complex<int>(2, 3)}},
    TMatrix{
        {std::complex<int>(0, 0), std::complex<int>(2, 3)},
        {std::complex<int>(0, 3), std::complex<int>(1, 0)}}, 1),
    TMatrix{
        {std::complex<int>(-12, 3), std::complex<int>(-4, 16)},
        {std::complex<int>(-9, 6), std::complex<int>(13, 26)}}
));

}

TEST(SecondLab, SingleThreadWithCorrectResults2) {
    ASSERT_TRUE(AreEqual(MultiplyMatrices(
        TMatrix{
            {std::complex<int>(3, 7), std::complex<int>(0, 4)}},
        TMatrix{
            {std::complex<int>(-1, -1)},
            {std::complex<int>(2, -3)}}, 1),
        TMatrix{
            {std::complex<int>(16, -2)}}
    )); 
}

TEST(SecondLab, SingleThreadWithCorrectResults3) {
    ASSERT_TRUE(AreEqual(MultiplyMatrices(
        TMatrix{
            {std::complex<int>(2, 3), std::complex<int>(7, 0)},
            {std::complex<int>(0, 0), std::complex<int>(0, 7)},
            {std::complex<int>(-7, 2), std::complex<int>(1, -7)},
            {std::complex<int>(2, 8), std::complex<int>(-1, -1)}},
        TMatrix{
            {std::complex<int>(0, 0), std::complex<int>(-3, -7), std::complex<int>(-2, 1)},
            {std::complex<int>(7, -4), std::complex<int>(0, -2), std::complex<int>(-1, -1)}}, 1),
        TMatrix{
            {std::complex<int>(49, -28), std::complex<int>(15, -37), std::complex<int>(-14, -11)},
            {std::complex<int>(28, 49), std::complex<int>(14, 0), std::complex<int>(7, -7)},
            {std::complex<int>(-21, -53), std::complex<int>(21, 41), std::complex<int>(4, -5)},
            {std::complex<int>(-11, -3), std::complex<int>(48, -36), std::complex<int>(-12, -12)}}
    )); 
}

TEST(SecondLab, ThreadConfigurations) {
    auto performTestForGivenSize = [](int m, int n, int p, int maxThreadCount) {
        auto mat1 = GenerateMatrix(m, n);
        auto mat2 = GenerateMatrix(n, p);
        auto result = MultiplyMatrices(mat1, mat2, 1);

        for(int i = 2; i < maxThreadCount; ++i) {
            ASSERT_TRUE(AreEqual(MultiplyMatrices(mat1, mat2, i), result));
        }
    };

    performTestForGivenSize(3, 4, 5, 10);
    performTestForGivenSize(10, 10, 10, 10);
    performTestForGivenSize(100, 100, 100, 15);
    performTestForGivenSize(500, 500, 500, 4);
}

TEST(SecondLab, PerfomanceTest) {
    auto getAvgTime = [](int threadCount) {
        auto m1 = GenerateMatrix(100, 500);
        auto m2 = GenerateMatrix(500, 300);

        constexpr int runsCount = 10;

        double avg = 0;

        for(int i = 0; i < runsCount; ++i) {
            auto begin = std::chrono::high_resolution_clock::now();
            MultiplyMatrices(m1, m2, threadCount);
            auto end = std::chrono::high_resolution_clock::now();
            avg += std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
        }

        return avg / runsCount;
    };

    auto singleThread = getAvgTime(1);
    auto multiThread = getAvgTime(4);

    std::cout << "Avg time for 1 thread: " << singleThread << '\n';
    std::cout << "Avg time for 4 threads: " << multiThread << '\n';

    EXPECT_GE(singleThread, multiThread);
}

int main(int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
