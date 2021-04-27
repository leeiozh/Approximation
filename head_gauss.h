//
// Created by leeiozh on 04.03.2021.
//

#ifndef APPROXIMATION_HEAD_GAUSS_H
#define APPROXIMATION_HEAD_GAUSS_H
#include "back_gauss.h"

template<typename T>
typename Vmatrix<T>::idx_t col_max(const Vmatrix<T> &A, const typename Vmatrix<T>::idx_t& col) {
    using idx_t = typename Vmatrix<T>::idx_t;
    using elm_t = typename Vmatrix<T>::elm_t;
    elm_t max = Tabs(A(col,col));
    int maxPos = col;
    for (idx_t i = col+1; i < A.sizeH(); ++i) {
        elm_t element = Tabs(A(i, col));
        if (element > max) {
            max = element;
            maxPos = i;
        }
    }
    return maxPos;
}

template <typename T>
unsigned int headTriangulation(Vmatrix<T> &A, std::vector<T> &b) {

    using idx_t = typename Vmatrix<T>::idx_t;
    using elm_t = typename Vmatrix<T>::elm_t;

    unsigned int swapCount = 0;

    for (idx_t i = 0; i < A.sizeH()-1; ++i) {
        idx_t imax = col_max(A, i);
        if(Tabs(A(imax, i)) < tolerance<T>){
            continue;
        }else {
            if (i != imax) {
                A.swap(i, imax);
                std::swap(b[i], b[imax]);
                ++swapCount;
            }

            for (idx_t k = i + 1; k < A.sizeH(); ++k) {
                elm_t coef = (A(k, i) / A(i, i));
                for (idx_t j = i; j < A.sizeW(); ++j) {
                    A(k, j) -= A(i, j) * coef;
                }
                b[k] -= b[i] * coef;
            }
        }
    }
    return swapCount;
}

template <typename T>
std::vector<T> head_gauss(Vmatrix<T> A, std::vector<T> b) {

    headTriangulation(A, b);
    return back_subst_top_triangular(A, b);
}


#endif //APPROXIMATION_HEAD_GAUSS_H
