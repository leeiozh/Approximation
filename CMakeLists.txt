cmake_minimum_required(VERSION 3.17)
project(approximation)

set(CMAKE_CXX_STANDARD 14)

add_executable(approximation main.cpp matrix.h gauss.h back_gauss.h head_gauss.h)