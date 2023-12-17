#include "lab2.hpp"
#include "utils.hpp"

// rows_lhs = isize(lhs);
// cols_lhs = isize(lhs[0]);
// rows_rhs = isize(rhs);
// cols_rhs = isize(rhs[0]);

struct MultiplyArgs {
    TMatrix* lhs = nullptr;
    TMatrix* rhs = nullptr;
    TMatrix* result = nullptr;
    int firstRow = 0;
    int lastRow = 0;
};

void ReadMatrix(int m, int n, TMatrix& matrix)  {
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            int real, imag;
            std::cin >> real >> imag;
            matrix[i][j] = std::complex<int>(real, imag);
        }
    }
}

void* MultiplyGivenRows(void* arguments) {
    auto &args = *(reinterpret_cast<MultiplyArgs *>(arguments));
    TMatrix lhs = *args.lhs;
    TMatrix &rhs = *args.rhs;
    TMatrix &result = *args.result;
    int firstRow = args.firstRow;
    int lastRow = args.lastRow;

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
    return nullptr;
}

TMatrix MultiplyMatrices(const TMatrix lhs, const TMatrix rhs, int threadCount) {
    TMatrix result(isize(lhs), std::vector<std::complex<int>>(isize(rhs[0])));

    if (threadCount > 1) {
        int actualThreads = std::min(threadCount, isize(result));
        std::vector<pthread_t> threads(actualThreads);
        std::vector<MultiplyArgs> args(actualThreads);
        int rowsPerThread = isize(result) / actualThreads;
        
        for (int i = 0; i < actualThreads; ++i) {
            int startRow = i * rowsPerThread;
            int endRow = (i == actualThreads - 1) ? isize(result) : startRow + rowsPerThread;
    
            args[i] = MultiplyArgs{(TMatrix *)&lhs, (TMatrix *)&rhs, &result, startRow, endRow};
            int rc = pthread_create(&threads[i], nullptr, MultiplyGivenRows, &args[i]);
            if (rc != 0) {
                throw std::runtime_error("Error creating thread");
            } 
        }
        for (int i = 0; i < actualThreads; ++i) {
            int rc = pthread_join(threads[i], nullptr);
            if (rc != 0) {
                throw std::runtime_error("Error joining thread");
            }
        }
    } else {
        MultiplyArgs args{(TMatrix *)&lhs, (TMatrix *)&rhs, &result, 0, isize(lhs)};
        MultiplyGivenRows(&args);
    }

    return result;
}

void PrintMatrix(int m, int n, TMatrix& matrix){
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
