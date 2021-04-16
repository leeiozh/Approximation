//
// Created by leeiozh on 14.04.2021.
//

#ifndef APPROXIMATION_GMRES_H
#define APPROXIMATION_GMRES_H

#include "csr.h"
#include "back_gauss.h"
#include "krylov.h"
#include <vector>

template<typename T>
Matrix<T> gmres(Matrix<T>& input, const Matrix<T>& b){
    CSR<T> A = input.to_csr();
    Matrix<T> x(b.getN(), 1);
    Matrix<T> r = b - (A * x.to_vector());
    int m = A.sizeW();
    T N = norm(r);
    std::vector<T> B;

    while (N > tolerance<T>){
        for (int i = 2; i <= m + 1; ++i){
            B.resize(i, 0);
            B[0] = N;

            auto [V, H] = KrylovSubSpace(A, r.to_vector(), i);
            HessenbergRot(H, B);
            if (Tabs(B.back()) < tolerance<T> || i == m + 1){
                H.delete_last_row();
                B.pop_back();
                x = x + V * back_subst_top_triangular(H, B);
                r = b - A * x.to_vector();
                N = norm(r);
                B.clear();
                break;
            }
            B.clear();
        }
    }
    return x;
}

#endif //APPROXIMATION_GMRES_H
