#include <iostream>
#include "Vmatrix.h"
#include "gmres.h"
#include "csr.h"

int main() {
    int n, m;

    std::cin >> n >> m;

    auto* arr = new double [9];

    for (int i = 0; i < n * m; i++){
        std::cin >> arr[i];
    }

    CSR<double> matr(n, m, to_csr(arr, n, m));

    std::vector<double> b(n);
    for (int i = 0; i < n; i++) {
        std::cin >> b[i];
    }

    std::cout << gmres(matr, b);
}

