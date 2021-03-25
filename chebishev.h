//
// Created by leeiozh on 12.03.2021.
//

#ifndef APPROXIMATION_CHEBISHEV_H
#define APPROXIMATION_CHEBISHEV_H
#include <ctgmath>
#include <vector>

template<typename T>
unsigned long long int C(int n, int k){
    unsigned long long int res = 1;
    for (int i = k +1; i <=n; ++i){
        res *= i;
    }
    for (int i = 1; i <= (n - k); ++i){
        res /= i;
    }
    return res;
}

template<typename T>
std::vector<T> cheb_roots(std::pair<T, T> section, size_t pow_two){
    size_t poly_order = std::pow(2, pow_two);
    std::vector<T> roots(poly_order);
    for(size_t i = 1; i <= poly_order; ++i){
        roots[i-1] = (section.first + section.second) / 2 + (section.second - section.first) / 2 * static_cast<T>(cos(static_cast<double>(2 * i - 1) * M_PI_2 / poly_order));
    }
    std::vector<size_t> idx;
    idx.push_back(0);
    idx.push_back(1);
    std::vector<size_t> next_idx;
    size_t cur_order;
    for(size_t i = 1; i < pow_two; ++i){
        cur_order = pow(2, double(i + 1));
        next_idx.resize(cur_order);
        for(size_t j = 0; j < cur_order - 1; j += 2){
            next_idx[j] = idx[j/2];
            next_idx[j + 1] = cur_order - 1 - next_idx[j];
        }
        idx = next_idx;
    }
    std::vector<T> res(roots.size());
    for(size_t i = 0; i < poly_order; ++i){
        res[i] = roots[idx[i]];
    }
    return res;
}

template<typename T>
std::vector<T> cheb_coefs(std::pair<T, T> sec, size_t poly_order){
    std::vector<T> coefs_f, coefs_s, coefs_new;

    coefs_f.emplace_back(static_cast<T>(1));
    coefs_s.emplace_back(static_cast<T>(0));
    coefs_s.emplace_back(static_cast<T>(1));

    for(size_t k = 1; k < poly_order; ++k){
        coefs_new.resize(k+2);
        for(size_t i = 0; i < k + 1; ++i){
            coefs_new[i] -= coefs_f[i];
            coefs_new[i + 1] += 2 * coefs_s[i];
            if(abs(coefs_new[i]) < tolerance) coefs_new[i] = static_cast<T>(0);
            if(abs(coefs_new[i + 1]) < tolerance) coefs_new[i] = static_cast<T>(0);
        }
        coefs_f = coefs_s;
        coefs_s = coefs_new;
        coefs_new.clear();
    }
    coefs_new.resize(poly_order + 1);
    for(int n = 0; n <= poly_order; ++n){
        for(int k = 0; k <= n; ++k){
            coefs_new[n-k] += coefs_s[n] * C<T>(n, k) * pow(-(sec.first + sec.second)/(sec.second - sec.first), k) * pow(static_cast<T>(2)/(sec.second-sec.first), n-k);
        }
    }
    T sum = static_cast<T>(0);
    for(auto i : coefs_new) sum += i;
    for(size_t i = 0; i <= poly_order; ++i) coefs_new[i]/=sum;

    return coefs_new;
}

#endif //APPROXIMATION_CHEBISHEV_H
