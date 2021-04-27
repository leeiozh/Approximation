//
// Created by leeiozh on 22.04.2021.
//

#ifndef APPROXIMATION_VMATRIX_H
#define APPROXIMATION_VMATRIX_H
#include <vector>
#include <set>
#include "math.h"

class Vmatrix_exception: public std::exception{
public:
    explicit Vmatrix_exception(std::string r): reason_{std::move(r)}{}

    char const* what() const noexcept {
        return reason_.c_str();
    }
private:
    std::string reason_;
};

template<typename T>
struct Triplet{
    std::size_t i;
    std::size_t j;
    T value;
    bool operator<(Triplet<T> const & rgh) const{
        return this->i<rgh.i || (this->i == rgh.i && this->j < rgh.j);
    }
};

template<typename T>
class Vmatrix{
public:
    using elm_t = T;
    using idx_t = std::size_t;

private:

    std::vector<T> matrix;
    idx_t H, W;

public:
    Vmatrix(const idx_t &h, const idx_t& w): H(h), W(w){
        if (h < 1 || w < 1) {
            throw Vmatrix_exception("ERROR! Nonpositive dimentions.");
        }
        matrix.resize(W*H);
    }

    Vmatrix(const idx_t &h, const idx_t& w, const std::set<Triplet<T>>& in): H(h), W(w){
        if (h < 1 || w < 1) {
            throw Vmatrix_exception("ERROR! Nonpositive dimentions.");
        }
        matrix.resize(W*H);
        for(auto i: in){
            matrix[i.i*W +i.j] = i.value;
        }
    }

    Vmatrix(const idx_t &h, const idx_t& w, const std::vector<T>& in):H(h), W(w){
        if (h < 1 || w < 1) {
            throw Vmatrix_exception("ERROR! Nonpositive dimentions.");
        }
        std::copy(in.begin(), in.end(), std::back_inserter(matrix));
    }

    elm_t& operator()(const idx_t& i, const idx_t& j){
        if (i >= H || j >= W) {
            throw Vmatrix_exception("ERROR! Out of range.");
        }
        return matrix[i * W + j];
    }
    const elm_t& operator()(const idx_t& i, const idx_t& j) const {
        if (i >= H || j >= W) {
            throw Vmatrix_exception("ERROR! Out of range.");
        }
        return matrix[i * W + j];
    }
    [[nodiscard]] const idx_t& sizeH() const {
        return H;
    }
    [[nodiscard]] const idx_t& sizeW() const {
        return W;
    }

    void swap(const idx_t& first, const idx_t& second){
        for(idx_t j = 0; j < W; ++j){
            std::swap(matrix[first * W + j], matrix[second * W + j]);
        }
    }

    void deleteLastRow(){
        --H;
        matrix.erase(matrix.end() - W, matrix.end());
    }

};

template<typename T>
std::vector<T> operator*(const Vmatrix<T>& A, const std::vector<T>& b){
    if (A.sizeW() != b.size()) {
        throw Vmatrix_exception("ERROR! Expected same dimensions.");
    }
    std::vector<T> res(b.size());
    T sum;
    for(size_t i = 0; i < A.sizeH(); ++i){
        sum = 0;
        for(size_t j = 0; j < A.sizeW(); ++j){
            sum+=A(i, j) * b[j];
        }
        res[i] = sum;
    }
    return res;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const Vmatrix<T>& A){
    for(std::size_t i = 0; i < A.sizeH(); ++i){
        for(std::size_t j = 0; j < A.sizeW(); ++j){
            os<<A(i, j)<<" ";
        }
        os<<std::endl;
    }
    return os;
}

template<typename T>
auto tolerance = static_cast<T>(1e-10);

template<typename T>
T Tabs(const T& a){
    if(a < T(0)) return -a;
    else return a;
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
std::vector<T> operator*(const T& k, const std::vector<T>& b){
    std::vector<T> res(b.size());
    for(size_t i = 0; i < b.size(); ++i){
        res[i] = b[i] * k;
    }
    return res;
}

template<typename T>
T operator*(const std::vector<T>& a, const std::vector<T>& b){
    if (a.size() != b.size) {
        throw Vmatrix_exception("ERROR! Expected same dimensions.");
    }
    T sum = 0;
    for(size_t i = 0; i < b.size(); ++i){
        sum += b[i] * a[i];
    }
    return sum;
}

template<typename T>
std::vector<T> operator-(const std::vector<T>& a, const std::vector<T>& b){
    return a + (-1) * b;
}

template<typename T>
std::vector<T> operator+(const std::vector<T>& a, const std::vector<T>& b){
    if (a.size() != b.size) {
        throw Vmatrix_exception("ERROR! Expected same dimensions.");
    }
    std::vector<T> res(a.size());
    for(size_t i = 0; i < a.size(); ++i){
        res[i] = a[i] + b[i];
    }
    return res;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& b){
    os<<"\n";
    for(auto i : b){
        os<<i<<" ";
    }
    return os;
}

#endif //APPROXIMATION_VMATRIX_H
