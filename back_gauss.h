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

template<typename T>
std::vector<T> back_subst_top_triangular(const Matrix<T>& A, const std::vector<T>& b){
    std::vector<T> res(b.size());
    res.back() = b.back()/A(A.getN()-1, A.getM()-1);
    T sum;
    for(int i = b.size() - 2; i >= 0; --i){
        sum = 0;
        for(size_t j = i + 1; j < b.size(); ++j){
            sum += A(i, j) * res[j];
        }
        res[i] = (b[i] - sum) / A(i, i);
    }
    return res;
}

template<typename T>
Matrix<T> back_subst_lower_triangular(const Matrix<T>& A, const Matrix<T>& b){
    Matrix<T> res(b.getN());
    res(0, 0) = b(0, 0) / A(0, 0);
    T sum;
    for(int i = 1; i < A.getN(); ++i){
        sum = 0;
        for(int j = 0; j < i; ++j){
            sum += A(i, j) * res(j, 0);
        }
        res(i, 0) = (b(i, 0) - sum) / A(i, i);
    }
    return res;
}

#endif //APPROXIMATION_BACK_GAUSS_H
