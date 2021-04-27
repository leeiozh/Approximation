#include <iostream>
#include "Vmatrix.h"
#include "gmres.h"
#include "csr.h"

int main() {
    int n, m;

    std::cin >> n >> m;

    std::set<Triplet<double>> in;
    for (size_t i = 0; i < n; i++){
        for (size_t j = 0; j < m; j++){
            double tmp;
            std::cin >> tmp;
            if (tolerance<double> < tmp){
                in.insert(Triplet<double> {i, j, tmp});
            }
        }
    }

    CSR<double> matr(n, m, in);

    std::vector<double> b(n);
    for (int i = 0; i < n; i++) {
        std::cin >> b[i];
    }

    std::cout << gmres(matr, b);
}

