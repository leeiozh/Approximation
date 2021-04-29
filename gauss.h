//
// Created by leeiozh on 24.02.2021.
//

#ifndef APPROXIMATION_GAUSS_H
#define APPROXIMATION_GAUSS_H
#include <exception>
#include <vector>
#include "Vmatrix.h"
#include "back_gauss.h"



template<typename T>
typename Vmatrix<T>::idx_t col_nonzero(const Vmatrix<T>& A, const typename Vmatrix<T>::idx_t& col){
    if(Tabs(A(col, col)) > tolerance<T>) return col;
    else {
        for(auto i = col + 1; i < A.sizeH(); ++i){
            if(Tabs( A(i, col) ) > tolerance<T>) return i;
        }
    }
    return col;
}

template<typename T>
void triangulation(Vmatrix<T>& A, std::vector<T>& b){
    using idx_t = typename Vmatrix<T>::idx_t;
    using elm_t = typename Vmatrix<T>::elm_t;

    for(idx_t i = 0; i < A.sizeH()-1; ++i){
        idx_t iNonZero = col_nonzero(A, i);
        if(Tabs(A(iNonZero, i)) > tolerance<T>){
            A.swap(i, iNonZero);
            std::swap(b[i], b[iNonZero]);
        }
        for(idx_t k = i + 1; k < A.sizeH(); ++k){
            elm_t coef = A(k, i) / A(i, i);
            for(idx_t j = 0; j < A.sizeW(); ++j){
                A(k, j) -= A(i, j) * coef;
            }
            b[k] -= b[i] * coef;
        }
    }
}
template<typename T>
std::vector<T> gauss(Vmatrix<T> A, std::vector<T> b){
    triangulation(A, b);
    return back_subst_top_triangular(A, b);
}

#endif //APPROXIMATION_GAUSS_H
