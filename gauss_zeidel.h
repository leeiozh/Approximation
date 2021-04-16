//
// Created by leeiozh on 19.03.2021.
//

#ifndef APPROXIMATION_GAUSS_ZEIDEL_H
#define APPROXIMATION_GAUSS_ZEIDEL_H

#include "csr.h"

template<typename T>
Matrix<T> gauss_zeidel(const CSR<T>& A, const Matrix<T>& b){

    Matrix<T> r(b.getN(), 1);
    Matrix<T> x(b.getN(), 1);

    T sum;
    r = A * x - b;
    int it = 0;
    while(norm(r) > tolerance<T>){
        for(size_t i = 0; i < A.H; ++i){
            sum = 0;
            for(size_t j = A.rows[i]; j < A.rows[i+1]; ++j){
                if( i != A.cols[j] ) sum += A.values[j] * x[A.cols[j]];
                else continue;
            }
            x[i] = (b[i] - sum)/A(i, i);
        }
        r = A * x - b;
        it++;
    }
    // std::cout << it <<"\n";
    return x;
}

#endif //APPROXIMATION_GAUSS_ZEIDEL_H
