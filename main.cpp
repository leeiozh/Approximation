#include <iostream>
#include <cmath>
#include "matrix.h"


int main() {
    int n, m;
    std::cin >> n >> m;
    double arr[n][m];
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            std::cin >> arr[i][j];
        }
    }
    Matrix test(*arr, n, m);
    std::cout << "------" << '\n';
    test.print();
    std::cout << "------" << '\n';
    Matrix minor_ = test.minor(0, 0);
    minor_.print();
    std::cout << "------" << '\n';
    std::cout << test.determinant() << '\n';
    std::cout << "------" << '\n';
    test.inverse().print();
}

