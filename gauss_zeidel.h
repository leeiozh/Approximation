//
// Created by leeiozh on 19.03.2021.
//

#ifndef APPROXIMATION_GAUSS_ZEIDEL_H
#define APPROXIMATION_GAUSS_ZEIDEL_H

#include "csr.h"

template<typename T>
std::vector<T> GaussSeidel(const CSR<T>& A, const std::vector<T>& b){
    using idx_t = typename CSR<T>::idx_t ;

    std::vector<T> r(b.size());
    std::vector<T> x(b.size());

    T sum;
    r = A * x - b;
    int it = 0;
    while(norm(r) > tolerance<T>){
        for(size_t i = 0; i < A.H; ++i){
            sum = 0;
            for(idx_t j = A.rows[i]; j < A.rows[i+1]; ++j){
                if( i != A.cols[j] ) sum += A.values[j] * x[A.cols[j]];
                else continue;
            }
            x[i] = (b[i] - sum)/A(i, i);
        }
        r = A * x - b;
        it++;
    }
    std::cout<<it<<std::endl;
    return x;
}
#endif //APPROXIMATION_GAUSS_ZEIDEL_H
