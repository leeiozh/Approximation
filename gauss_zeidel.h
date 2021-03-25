//
// Created by leeiozh on 19.03.2021.
//

#ifndef APPROXIMATION_GAUSS_ZEIDEL_H
#define APPROXIMATION_GAUSS_ZEIDEL_H

template<typename T>
Matrix<T> GaussSeidel(const Matrix<T>& A, const Matrix<T>& b){

    Matrix<T> r(b.getN(), 1);
    Matrix<T> x(b.getN(), 1);

    T sum;
    r = A * x - b;

    while (norm(r) > tolerance){
        for (auto i = 0; i < A.getN(); ++i){
            sum = 0;
            for (auto j = 0; j < A.getN(); ++j){
                if (i != j) sum += A(i, j) * x(j, 0);
                else continue;
            }
            x(i, 0) = (b(i, 0) - sum) / A(i, i);
        }
        r = A * x - b;
    }
    return x;
}

#endif //APPROXIMATION_GAUSS_ZEIDEL_H
