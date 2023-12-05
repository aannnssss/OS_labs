#include "lab2.hpp"
#include "utils.hpp"

void ReadMatrix(int m, int n, TMatrix& matrix)  {
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            int real, imag;
            std::cin >> real >> imag;
            matrix[i][j] = std::complex<int>(real, imag);
        }
    }
}
// rows_lhs = isize(lhs);
// cols_lhs = isize(lhs[0]);
// rows_rhs = isize(rhs);
// cols_rhs = isize(rhs[0]);

namespace {
    void MultiplyGivenRows(const TMatrix& lhs, const TMatrix& rhs, TMatrix& result, int firstRow, int lastRow) {
        int m = isize(rhs);
        int p = isize(rhs[0]);

        if (lastRow == -1) {
            lastRow = isize(lhs);
        }

        for (int i = firstRow; i < lastRow; ++i) {
            for (int j = 0; j < p; ++j) {
                for (int k = 0; k < m; ++k) {
                    result[i][j] += lhs[i][k] * rhs[k][j];
                }
            }
        }
    }    
}

TMatrix MultiplyMatrices(const TMatrix& lhs, const TMatrix& rhs, int threadCount) {
    TMatrix result(isize(lhs), std::vector<std::complex<int>>(isize(rhs[0])));

    if(threadCount > 1) {
        int actualThreads = std::min(threadCount, isize(result));

        std::vector<std::thread> threads;
        threads.reserve(actualThreads);

        int rowsPerThread = isize(result) / actualThreads;

        for(int i = 0; i < isize(result); i += rowsPerThread) {
            if(i + rowsPerThread >= isize(result)) {
                threads.emplace_back(MultiplyGivenRows, std::ref(lhs), std::ref(rhs), std::ref(result), i, isize(result));
            } else {
                threads.emplace_back(MultiplyGivenRows, std::ref(lhs), std::ref(rhs), std::ref(result), i, i + rowsPerThread);
            }
        }

        for(auto& thread : threads) {
            thread.join();
        }
    } else {
        MultiplyGivenRows(lhs, rhs, result, 0, isize(lhs));
    }
    return result;
}

void PrintMatrix(int m, int n, const TMatrix& matrix){
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (matrix[i][j].real() != 0) {
                std::cout << " " << matrix[i][j].real();
            }
            if (matrix[i][j].imag() != 0) {
                if (matrix[i][j].imag() > 0 && matrix[i][j].real() != 0) {
                    std::cout << "+";
                }
                if (matrix[i][j].imag() == 1){ 
                    std::cout << "i ";
                } else if (matrix[i][j].imag() == -1) {
                    std::cout << "-i ";
                } else std::cout << matrix[i][j].imag() << "i" << " ";
            }
            if (matrix[i][j].real() == 0 && matrix[i][j].imag() == 0) {
                std::cout << " 0 ";
            }
        }
        std::cout << std::endl;
    }
}
