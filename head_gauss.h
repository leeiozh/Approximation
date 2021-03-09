//
// Created by leeiozh on 04.03.2021.
//

#ifndef APPROXIMATION_HEAD_GAUSS_H
#define APPROXIMATION_HEAD_GAUSS_H
#include "back_gauss.h"

class head_gauss_exception: public std::exception{
public:
    explicit head_gauss_exception(std::string r): reason_{std::move(r)}{}

    char const* what() const noexcept {
        return reason_.c_str();
    }
private:
    std::string reason_;
};

template<typename T>

int col_max(Matrix<T> &A, int& col){
    auto max = abs(A(col, col));
    int max_pos = col;
    for (auto i = col + 1; i < A.getN(); i++){
        auto curr = abs(A(i, col));
        if (curr > max) {
            max = curr;
            max_pos = i;
        }
    }
    return max_pos;
}

template <typename T>
int headTriangulation(Matrix<T> &A, Matrix<T> &b){
    if (!A.is_square()) throw head_gauss_exception("Warning! Gauss method can be used only for square matrix");
    if (b.getM() != 1) throw head_gauss_exception("Error: b is not a one column");
    if (A.getN() != b.getN()) throw head_gauss_exception("Warning! Matrix and free column must have same dimensions");

    int swapCount = 0;
    int n = A.getN();

    for (auto i = 0; i < n; i++){
        auto i_max = col_max(A, i);
        if (abs(A(i_max, i)) < tolerance) throw head_gauss_exception("Error: A is degenerate");
        if (i != i_max){
            A.swap(i, i_max);
            b.swap(i, i_max);
            swapCount++;
        }

        for (auto k = i + 1; k < n; k++){
            auto coeff = A(k, i) / A(i, i);
            for (auto j = i; j < n; j++){
                A(k, j) -= A(i, i) * coeff;
            }
            b(k, 0)-= b(i, 0) * coeff;
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
