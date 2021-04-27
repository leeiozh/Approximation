//
// Created by leeiozh on 12.03.2021.
//

#ifndef APPROXIMATION_SIMPLE_ITERATION_H
#define APPROXIMATION_SIMPLE_ITERATION_H
#include <vector>
#include "csr.h"

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
std::vector<T> simple_iteration(const CSR<T>& A, const std::vector<T>& b, const std::vector<T>& ChebRoots){
    std::vector<T> x(b.size());
    std::vector<T> r = b - A * x;

    T N = norm(r);
    bool flag = true;
    int i =0;
    while(flag){
        for(auto root : ChebRoots){
            x = -static_cast<T>(1)/root * r + x;
            r = b - A * x;
            N = norm(r);
            ++i;
            if( N < tolerance<T>){
                flag = false;
                break;
            }
        }
    }
    std::cout<<i<<std::endl;
    return x;
}


#endif //APPROXIMATION_SIMPLE_ITERATION_H
