//
// Created by leeiozh on 24.02.2021.
//

#ifndef APPROXIMATION_BACK_GAUSS_H
#define APPROXIMATION_BACK_GAUSS_H

template <typename T>

Matrix<T> back_gauss(Matrix<T>& A, Matrix<T>& b){
    assert(A.is_square() && "Gauss method can be used only for square matrix ");
    assert(b.get_dimensions().second == 1 && "Error, b is not 1 columns");
    assert(A.get_dimensions().first == b.get_dimensions().first && "Matrix and column must have same dimensions");

    int n = A.get_dimensions().first;
    double * res = new double [n];
    Matrix<T> result (res, n, 1);
    result.data[n - 1][0] = b.data[n - 1][0] / A.data[n - 1][n - 1];
    double sum;
    for (int i = n - 2; i >= 0; i--){
        sum = 0;
        for (int j = i + 1; j < n; j++){
            sum += A.data[i][j] * result.data[j][0];
        }
        result.data[i][0] = (b.data[i][0] - sum) / A.data[i][i];
    }
    return result;
}

#endif //APPROXIMATION_BACK_GAUSS_H
