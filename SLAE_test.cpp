#include <iostream>
#include "Vmatrix.h"
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
    std::vector<double> v;
    v.resize(n * m);

    for (int i = 0; i < n * m; i++){
        std::cin >> v[i];
    }

    /*std::set<Triplet<double>> in;
    for (size_t i = 0; i < n; i++){
        for (size_t j = 0; j < m; j++){
            double tmp;
            std::cin >> tmp;
            if (tolerance<double> < tmp){
                in.insert(Triplet<double> {i, j, tmp});
            }
        }
    }*/

    std::vector<double> b;
    b.resize(n);
    for (int i = 0; i < n; i++) {
        std::cin >> b[i];
    }

    Vmatrix<double> test(n, m, v);

    std::cout << head_gauss(Vmatrix<double>(test), b);
}

