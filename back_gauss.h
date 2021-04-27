//
// Created by leeiozh on 24.02.2021.
//

#ifndef APPROXIMATION_BACK_GAUSS_H
#define APPROXIMATION_BACK_GAUSS_H

template <typename T>
std::vector<T> back_subst_top_triangular(const Vmatrix<T>& A, const std::vector<T>& b){
    std::vector<T> res(b.size());
    res.back() = b.back()/A(A.sizeH()-1, A.sizeW()-1);
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
std::vector<T> back_subst_lower_triangular(const Vmatrix<T>& A, const std::vector<T>& b){
    std::vector<T> res(b.size());
    res[0] = b[0] / A(0, 0);
    T sum;
    for(int i = 1; i < A.sizeH(); ++i){
        sum = 0;
        for(int j = 0; j < i; ++j){
            sum += A(i, j) * res[j];
        }
        res[i] = (b[i] - sum) / A(i, i);
    }
    return res;
}

#endif //APPROXIMATION_BACK_GAUSS_H
