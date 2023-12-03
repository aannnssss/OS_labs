#ifndef OS_LABS_LAB2_H
#define OS_LABS_LAB2_H

#include <vector>
#include <complex>
#include <thread>
#include <iostream>

typedef std::vector<std::vector<std::complex<int> > > TMatrix;

void readMatrix(int m, int n, TMatrix& matrix);

TMatrix MultiplyMatrices(const TMatrix& lhs, const TMatrix& rhs, int threadCount);

void printMatrix(int m, int n, const TMatrix& matrix);

#endif //OS_LABS_LAB2_H