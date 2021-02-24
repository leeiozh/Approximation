#include <iostream>
#include "matrix.h"
#include "gauss.h"

int main() {
    int n, m;
    std::cin >> n >> m;
    double arr[n][m];
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            std::cin >> arr[i][j];
        }
    }
    double b[n][1];
    for (int i = 0; i < n; i++) {
        std::cin >> b[i][0];
    }
    Matrix test(*arr, n, m);
    Matrix best(*b, n, 1);
    std::cout << "------" << '\n';
    test.print();
    std::cout << "------" << '\n';
    Matrix result = gauss(test, best);
    result.print();
}

