//
// Created by leeiozh on 04.03.2021.
//

#ifndef APPROXIMATION_HEAD_GAUSS_H
#define APPROXIMATION_HEAD_GAUSS_H
#include "back_gauss.h"

template<typename T>

int col_max(Matrix<T> &A, int& col){
    auto max = Tabs(A(col, col));
    int max_pos = col;
    for (auto i = col + 1; i < A.get_dimensions().first; i++){
        auto curr = Tabs(A(i, col));
        if (curr > max) {
            max = curr;
            max_pos = i;
        }
    }
    return max_pos;
}

template <typename T>
int headTriangulation(Matrix<T> &A, Matrix<T> &b){
    assert(A.is_square() && "Gauss method can be used only for square matrix ");
    assert(b.get_dimensions().second == 1 && "Error, b is not 1 columns");
    assert(A.get_dimensions().first == b.get_dimensions().first && "Matrix and free column must have same dimensions");

    int swapCount = 0;
    int n = A.get_dimensions().first;

    for (auto i = 0; i < n; i++){
        auto i_max = col_max(A, i);
        assert (Tabs(A(i_max, i)) > 1e-10 && "A is degenerate");
        if (i != i_max){
            A.swap(i, i_max);
            b.swap(i, i_max);
            swapCount++;
        }

        for (auto k = i + 1; k < n; k++){
            auto coeff = A(k, i) / A(i, i);
            for (auto j = i; j < n; j++){
                A.data[k][j] -= A(i, i) * coeff;
            }
            b.data[k][0] -= b(i, 0) * coeff;
        }
    }
    return swapCount;
}

template <typename T>
Matrix<T> head_gauss(Matrix<T> &A, Matrix<T> &b){
    headTriangulation(A, b);
    return back_gauss(A, b);
}

#endif //APPROXIMATION_HEAD_GAUSS_H
