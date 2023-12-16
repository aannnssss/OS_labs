#pragma once

#include <vector>
#include <complex>
#include <pthread.h>
#include <iostream>
#include <array>

using TMatrix = std::vector<std::vector<std::complex<int>>>;

void ReadMatrix(int m, int n, TMatrix& matrix);

void* MultiplyGivenRows(void* args);

TMatrix MultiplyMatrices(const TMatrix lhs, const TMatrix rhs, int threadCount);

void PrintMatrix(int m, int n, TMatrix& matrix);