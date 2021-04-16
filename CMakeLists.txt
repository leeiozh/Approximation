cmake_minimum_required(VERSION 3.17)
project(approximation)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-fsanitize=address")
add_definitions(-D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC)
set(CMAKE_CXX_LINK_LIBRARY_FLAG "-fsanitize=address")
add_executable(approximation SLAE_test.cpp matrix.h gauss.h back_gauss.h head_gauss.h simple_iteration.h chebishev.h gauss_zeidel.h csr.h krylov.h gmres.h)