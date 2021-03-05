//
// Created by leeiozh on 24.02.2021.
//

#ifndef APPROXIMATION_GAUSS_H
#define APPROXIMATION_GAUSS_H
#include "back_gauss.h"

template<typename T>

int col_non_zero(Matrix<T>& A, const int col){
    if (Tabs(A.data[col][col] > 1e-10)) return col;
    else {
        for (auto i = col + 1; i < A.get_dimensions().first; i++){
            if (A.data[i][col] > 1e-10) return i;
        }
    }
    return col;
}

template<typename T>

int triangulation(Matrix<T>& A, Matrix<T>& b){
    assert(A.is_square() && "Gauss method can be used only for square matrix ");
    assert(b.get_dimensions().second == 1 && "Error, b is not 1 columns");
    assert(A.get_dimensions().first == b.get_dimensions().first && "Matrix and free column must have same dimensions");

    int swap_count = 0;
    int n = A.get_dimensions().first;

    for (int i = 0; i < n - 1; i++){
        auto c = col_non_zero(A, i);
        assert(Tabs(A.data[c][i]) > 1e-10 && "A is degenerate\n");
        if (i != c){
            A.swap(i, c);
            b.swap(i, c);
            swap_count++;
        }
        for (int k = i + 1; k < n; k++){
            auto coeff = A.data[k][i] / A.data[i][i];
            for (auto s = 0; s < n; s++){
                A.data[k][s] -= A.data[i][s] * coeff;
            }
            b.data[k][0] -= b.data[i][0] * coeff;
        }
    }
    return swap_count;
}


template<typename T>

Matrix<T> gauss(Matrix<T> &A, Matrix<T> &b){
    triangulation(A, b);
    return back_gauss(A, b);
}

#endif //APPROXIMATION_GAUSS_H
