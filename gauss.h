//
// Created by leeiozh on 24.02.2021.
//

#ifndef APPROXIMATION_GAUSS_H
#define APPROXIMATION_GAUSS_H
#include "back_gauss.h"
#include <cmath>

int col_non_zero(Matrix& A, const int col){
    if (std::abs(A.data[col][col]) > 1e-10) return col;
    else {
        for (auto i = col + 1; i < A.get_dimensions().first; i++){
            if (A.data[i][col] > 1e-10) return i;
        }
    }
    return col;
}

int triangulation(Matrix& A, Matrix& b){

    if (b.get_dimensions().second != 1){
        std::cout << "Error, b has not 1 columns";
    } else {

        int swap_count = 0;

        for (int i = 0; i < A.get_dimensions().first - 1; i++){
            if (std::abs(A.data[col_non_zero(A, i)][i]) > 1e-10) {
                A.swap(i, col_non_zero(A, i));
                b.swap(i, col_non_zero(A, i));
                swap_count ++;
            } else {
                std::cout << "A is degenerate\n";};
            for (int k = i + 1; k < A.get_dimensions().first; k++){
                auto coeff = A.data[k][i] / A.data[i][i];
                for (auto s = 0; s < A.get_dimensions().second; s++){
                    A.data[k][s] -= A.data[i][s] * coeff;
                }
                b.data[k][0] -= b.data[i][0] * coeff;
            }
        }
        return swap_count;
    }
}

Matrix gauss(Matrix A, Matrix b){
    triangulation(A, b);
    return back_gauss(A, b);
}

#endif //APPROXIMATION_GAUSS_H
