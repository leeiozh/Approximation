#include <iostream>
#include "Vmatrix.h"
#include "gmres.h"
#include "csr.h"

int main() {

    int n , m;

    std::cin >> n >> m;

    auto* arr = new double [n*m];

    for (int i = 0; i < n*m; i++){
        std::cin >> arr[i];
    }

    auto* b = new double [n];

    for (int i = 0; i < n; i++){
        std::cin >> b[i];
    }

    std::vector<double> results(n);

    for (int i = 0; i < n; i++){
        results[i] = b[i];
    }
    std::vector<double> ans = gmres(CSR<double> (n, m, to_tripl(arr, n, m)), results);

    delete [] arr;
    delete [] b;

    for (int i = 0; i < n; i++){
        std::cout << " "<< ans[i];
    }
}

