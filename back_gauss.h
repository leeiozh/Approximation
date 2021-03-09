//
// Created by leeiozh on 24.02.2021.
//

#ifndef APPROXIMATION_BACK_GAUSS_H
#define APPROXIMATION_BACK_GAUSS_H

template <typename T>

Matrix<T> back_gauss(Matrix<T>& A, Matrix<T>& b){

    int n = A.getN();
    Matrix<T> result (n, 1);
    result(n - 1, 0) = b(n - 1, 0) / A(n - 1, n - 1);
    double sum;
    for (int i = n - 2; i >= 0; i--){
        sum = 0;
        for (int j = i + 1; j < n; j++){
            sum += A(i, j) * result(j, 0);
        }
        result(i, 0) = (b(i, 0) - sum) / A(i, i);
    }
    return result;
}

#endif //APPROXIMATION_BACK_GAUSS_H
