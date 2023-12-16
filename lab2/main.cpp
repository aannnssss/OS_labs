#include "lab2.hpp"

int main() {
    int n1, m1, n2, m2, threadCount;

    std::cout << "Input values rows and columns for matrix 1: ";
    std::cin >> m1 >> n1;
    TMatrix lhs(m1, std::vector<std::complex<int> >(n1));
    std::cout << "Input matrix 1:\nexample: if you want to input 2+3i => input 2 3\n";
    ReadMatrix(m1, n1, lhs);

    std::cout << "Input values rows and columns for matrix 2: ";
    std::cin >> m2 >> n2;

    if (n1 != m2) {
        printf("Error: the value of columns in the first matrix does not match the value of the rows in the second matrix\n");
        return 0;
    }

    TMatrix rhs(m2, std::vector<std::complex<int> >(n2));
    std::cout << "Input matrix 2:\nexample: if you want to input 2+3i => input 2 3\n";
    ReadMatrix(m2, n2, rhs);

    std::cout << "input value of threads: ";
    std::cin >> threadCount;

    TMatrix result = MultiplyMatrices(lhs, rhs, threadCount);

    PrintMatrix(m1, n2, result);
}