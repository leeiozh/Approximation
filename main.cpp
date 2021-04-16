#include <iostream>
#include "matrix.h"
#include "gauss.h"
#include "head_gauss.h"
#include "simple_iteration.h"
#include "chebishev.h"
#include "gauss_zeidel.h"
#include "gmres.h"
#include "csr.h"

int main() {

    int n, m;
    std::cin >> n >> m;

    double *arr = new double [m * n];

    for (int i = 0; i < n * m; ++i){
        std::cin >> arr[i];
    }

    Matrix<double> test(arr, n, m);

    std::vector<double> b;
    b.resize(n);
    for (int i = 0; i < n; i++) {
        std::cin >> b[i];
    }

    Matrix<double> best(b);
    std::cout << gmres(test, best);
}

