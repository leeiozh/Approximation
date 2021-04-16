//
// Created by leeiozh on 14.04.2021.
//

#ifndef APPROXIMATION_KRYLOV_H
#define APPROXIMATION_KRYLOV_H

#include "csr.h"
#include <tuple>
#include <vector>

template<typename T>
void HessenbergRot(Matrix<T>& H, std::vector<T>& b){
    T cos;
    T sin;
    T up, down;
    T Bup, Bdown;
    for(size_t k = 0; k < H.getM(); ++k){
        cos = H(k, k) / sqrt(H(k, k) * H(k, k) + H(k + 1, k) * H(k + 1, k));
        sin = H(k + 1, k) / sqrt(H(k, k) * H(k, k) + H(k + 1, k) * H(k + 1, k));
        Bup = cos * b[k] + sin * b[k + 1];
        Bdown = -sin * b[k] + cos * b[k + 1];
        b[k] = Bup;
        b[k + 1] = Bdown;
        for(size_t j = 0; j < H.getM(); ++j){
            up = cos * H(k, j) + sin * H(k + 1, j);
            down = -sin * H(k, j) + cos * H(k + 1, j);
            if(Tabs(down) < tolerance<T>) down = 0;
            H(k, j) = up;
            H(k + 1, j) = down;
        }
    }
}

template<typename T>
T norm(const std::vector<T>& a){
    T sum = 0;
    for(size_t i = 0; i < a.size(); ++i){
        sum += a[i]*a[i];
    }
    return sqrt(sum);
}

template<typename T>
T operator *(const std::vector<T>& a, std::vector<T>& c){
    T res = 0;
    for (auto i = 0; i < a.size(); i++){
        res += a[i] * c[i];
    }
    return res;
}

template<typename T>
std::vector<T> operator *(const T& a, std::vector<T>& c){
    std::vector<T> res(c.size());
    for (auto i = 0; i < c.size(); i++){
        res[i] = a * c[i];
    }
    return res;
}

template<typename T>
std::vector<T> operator -(const std::vector<T>& a, const std::vector<T>& b){
    std::vector<T> res(a.size());
    for (auto i = 0; i < a.size(); i++){
        res[i] = a[i] - b[i];
    }
    return res;
}


template<typename T>
std::tuple<Matrix<T>, Matrix<T>> KrylovSubSpace(const CSR<T>& A, const std::vector<T>& r, size_t N){

    std::set<Triplet<T>> H;
    std::vector<std::vector<T>> Basis;
    Matrix<T> V(r.size(), N-1);
    std::vector<T> v = r;

    v = 1./norm(v) * v;
    Basis.push_back(v);
    T h;
    for (size_t j = 0; j < N-1; ++j){
        v = A * Basis[j];
        for (size_t i = 0; i <= j; ++i){
            h = Basis[i] * v;
            v = v - h * Basis[i];
            if (Tabs(h) > tolerance<T>) H.insert({i, j, h});
        }
        h = norm(v);
        if (Tabs(h) > tolerance<T>) H.insert({j + 1, j, h});
        v = 1./ h * v;
        Basis.push_back(v);
    }
    for (size_t j = 0; j < N-1; ++j){
        for (size_t i = 0; i < r.size(); ++i){
            V(i, j) = Basis[j][i];
        }
    }
    return std::tuple<Matrix<T>, Matrix<T>>(V, Matrix<T>(N, N - 1, H));
}

#endif //APPROXIMATION_KRYLOV_H
