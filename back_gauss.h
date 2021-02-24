//
// Created by leeiozh on 24.02.2021.
//

#ifndef APPROXIMATION_BACK_GAUSS_H
#define APPROXIMATION_BACK_GAUSS_H

Matrix back_gauss(Matrix& A, Matrix& b){
    int n = A.get_dimensions().first;
    double * res = new double [n];
    Matrix result (res, n, 1);
    result.data[n - 1][0] = b.data[n - 1][0] / A.data[n - 1][n - 1];
    double sum;
    for (int i = n - 2; i >= 0; i--){
        sum = 0;
        for (int j = j + 1; j < n; j ++){
            sum += A.data[i][j] * result.data[j][0];
        }
        result.data[i][0] = (b.data[i][0] - sum) / A.data[i][i];
    }
    return result;
}

#endif //APPROXIMATION_BACK_GAUSS_H
