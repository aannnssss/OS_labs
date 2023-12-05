#ifndef OS_LABS_LAB2_H
#define OS_LABS_LAB2_H

#include <vector>
#include <complex>
#include <pthread.h>
#include <thread>
#include <iostream>
#include <array>

using TMatrix = std::vector<std::vector<std::complex<int>>>;

void ReadMatrix(int m, int n, TMatrix& matrix);

TMatrix MultiplyMatrices(const TMatrix& lhs, const TMatrix& rhs, int threadCount);

void PrintMatrix(int m, int n, const TMatrix& matrix);

#endif //OS_LABS_LAB2_H