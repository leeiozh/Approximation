//
// Created by leeiozh on 14.04.2021.
//

#ifndef APPROXIMATION_CSR_H
#define APPROXIMATION_CSR_H
#include <set>

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
auto tolerance = static_cast<T>(1e-10);

template<typename T>
class CSR{
public:
    using elm_t = T;
    using idx_t = std::size_t;
private:
    const idx_t H, W;
    std::vector<elm_t> values;
    std::vector<idx_t> cols;
    std::vector<idx_t> rows;

public:
    CSR(const idx_t &h, const idx_t &w, const std::set<Triplet<elm_t>>& in): H(h), W(w){
        values.resize(in.size());
        cols.resize(in.size());
        rows.resize(h+1);
        int countInRow = 0;
        int currRow = 0;
        auto it = in.begin();
        for(idx_t k = 0; k < in.size(); ++k){
            while(currRow < it->i){
                rows[currRow+1] = rows[currRow] + countInRow;
                ++currRow;
                countInRow = 0;
            }
            values[k] = it->value;
            cols[k] = it->j;
            ++countInRow;
            it = next(it);
        }
        for(++currRow; currRow <= H; ++currRow){
            rows[currRow] = in.size();
        }
    }
    [[nodiscard]] idx_t sizeH() const{
        return H;
    }
    [[nodiscard]] idx_t sizeW() const{
        return W;
    }

    std::vector<elm_t> operator*(const std::vector<elm_t> &b) const{
        std::vector<elm_t> res(this->H);
        for(idx_t i = 0; i < H; ++i){
            for(idx_t j = rows[i]; j < rows[i+1]; ++j) res[i] += values[j] * b[cols[j]];
        }
        return res;
    }

    elm_t operator()(idx_t const i, idx_t const j) const{
        idx_t skip = this->rows[i];
        idx_t count = this->rows[i+1] - skip;
        for(idx_t k = skip; k < skip + count; ++k){
            if(this->cols[k] == j) return this->values[k];
        }
        return static_cast<elm_t>(0);
    }

};
template<typename T>
std::ostream& operator<<(std::ostream& os, const CSR<T>& A){
    os<<std::endl;
    for(std::size_t i = 0; i < A.sizeH(); ++i){
        for(std::size_t j = 0; j < A.sizeW(); ++j){
            os<<A(i, j)<<" ";
        }
        os<<std::endl;
    }
    return os;
}

template<typename T>
std::set<Triplet<T>> to_tripl(T* arr, int n, int m) {
    std::set<Triplet<T>> in;
    for (size_t i = 0; i < n; i++){
        for (size_t j = 0; j < m; j++){
            if (tolerance<T> < arr[i * n + j]){
                in.insert(Triplet<T> {i, j, arr[i * n + j]});
            }
        }
    }
    return in;
}

#endif //APPROXIMATION_CSR_H
