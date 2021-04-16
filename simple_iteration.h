//
// Created by leeiozh on 12.03.2021.
//

#ifndef APPROXIMATION_SIMPLE_ITERATION_H
#define APPROXIMATION_SIMPLE_ITERATION_H
#include <vector>

class simple_iteration_exception: public std::exception{
public:
    explicit simple_iteration_exception(std::string r): reason_{std::move(r)}{}

    char const* what() const noexcept override {
        return reason_.c_str();
    }
private:
    std::string reason_;
};

template<typename T>
Matrix<T> simple_iteration(const Matrix<T>& A, const Matrix<T> &b, std::vector<T>& cheb_roots){
    if (A(1, 2) != A(2, 1)) throw  simple_iteration_exception("Warning! Simple iteration method worked only with symmetric matrix ");
    if (!A.is_square()) throw simple_iteration_exception("Warning! Simple iteration can be used only for square matrix");
    if (b.getM() != 1) throw simple_iteration_exception("Error: b is not a one column");
    if (A.getN() != b.getN()) throw simple_iteration_exception("Warning! Matrix and free column must have same dimensions");

    Matrix<T> x (A.getN(), 1);
    Matrix<T> r = b - A * x;

    T N = norm(r);
    bool flag = true;
    int i = 0;
    while (flag) {
        for (auto root: cheb_roots){
            x = x - r / root;
            r = b - A * x;
            N = norm(r);
            ++i;
            if (N < tolerance<T>){
                flag = false;
                break;
            }
        }
    }
    return x;
}

#endif //APPROXIMATION_SIMPLE_ITERATION_H
