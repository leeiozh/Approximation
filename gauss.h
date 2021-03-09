//
// Created by leeiozh on 24.02.2021.
//

#ifndef APPROXIMATION_GAUSS_H
#define APPROXIMATION_GAUSS_H
#include <exception>
#include "back_gauss.h"

class gauss_exception: public std::exception{
public:
    explicit gauss_exception(std::string r): reason_{std::move(r)}{}

    char const* what() const noexcept {
        return reason_.c_str();
    }
private:
    std::string reason_;
};

double const tolerance = 1e-10;

template<typename T>

int col_non_zero(Matrix<T>& A, const int col){
    if (abs(A(col, col) > tolerance)) return col;
    else {
        for (auto i = col + 1; i < A.getN(); i++){
            if (A(i, col) > tolerance) return i;
        }
    }
    return col;
}

template<typename T>

int triangulation(Matrix<T>& A, Matrix<T>& b){
    if (!A.is_square()) throw gauss_exception("Warning! Gauss method can be used only for square matrix");
    if (b.getM() != 1) throw gauss_exception("Error: b is not a one column");
    if (A.getN() != b.getN()) throw gauss_exception("Warning! Matrix and free column must have same dimensions");

    int swap_count = 0;
    int n = A.getN();

    for (int i = 0; i < n - 1; i++){
        auto c = col_non_zero(A, i);
        if (abs(A(c, i)) < tolerance) throw gauss_exception("Error: A is degenerate");
        if (i != c){
            A.swap(i, c);
            b.swap(i, c);
            swap_count++;
        }
        for (int k = i + 1; k < n; k++){
            auto coeff = A(k, i)/ A(i, i);
            for (auto s = 0; s < n; s++){
                A(k, s) -= A(i, s) * coeff;
            }
            b(k, 0) -= b(i, 0) * coeff;
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
