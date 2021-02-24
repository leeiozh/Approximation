//
// Created by leeiozh on 24.02.2021.
//

#ifndef APPROXIMATION_MATRIX_H
#define APPROXIMATION_MATRIX_H
#include <cmath>

class Matrix {
public:
    double** data;

private:
    int n, m;

public:

    Matrix(double* arr, int n, int m) {
        this->n = n;
        this->m = m;
        this->data = new double*[n];
        for (int i = 0; i < n; i++) {
            this->data[i] = new double[m];
            for (int j = 0; j < m; j++) {
                this->data[i][j] = arr[i * m + j];
            }
        }
    }

    ~Matrix() {
        for (int i = 0; i < n; i++){
            delete [] data[i];
        }
        delete[] data;
    }

    void print() {
        for (int i = 0; i < this->n; i++) {
            for (int j = 0; j < this->m; j++) {
                std::cout << this->data[i][j] << ' ';
            }
            std::cout << '\n';
        }
    }

    void swap(int i, int j) {
        if (i < this->n && j < this->n){
            for (int c = 0; c < this->m; c++){
                auto tmp = data[j][c];
                data[j][c] = data[i][c];
                data[i][c] = tmp;
            }
        } else {
            std::cout << "Error swap \n";
        }
    }

    std::pair <int, int> get_dimensions() {
        return std::make_pair(this->n, this->m);
    }

    bool is_square() {
        if (this->n == this->m) {
            return true;
        }
        return false;
    }

    Matrix get_submatrix(int start_str, int start_col, int n, int m) {
        double ans[n][m];
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                ans[i][j] = this->data[start_str + i][start_col + j];
            }
        }
        Matrix submatrix(*ans, n, m);
        return submatrix;
    }

    double algebraic_addition(int i, int j) {
        double sign = -1;
        if ((i + j) % 2 == 0) {
            sign = 1;
        }
        return (this->minor(i, j).determinant()) * sign;
    }

    Matrix inverse() {
        if (!this->is_square()) {
            std::cout << "WARNING! The inverse matrix is defined only for square matrices." << '\n';
            double arr[1][1];
            arr[0][0] = nan("1");
            Matrix ans(*arr, 1, 1);
            return ans;
        }
        double arr[this->n][this->m];
        double det = this->determinant();
        for (int i = 0; i < this->n; i++) {
            for (int j = 0; j < this->m; j++) {
                arr[i][j] = this->algebraic_addition(i, j) / det;
            }
        }
        Matrix ans(*arr, this->n, this->m);
        return ans;
    }

    Matrix minor(int i, int j) {
        double arr[this->n - 1][this->m - 1];
        int str = 0, col = 0;
        for (int _i = 0; _i < this->n; _i++) {
            if (_i == i) {
                str = 1;
                continue;
            }
            for (int _j = 0; _j < this->m; _j++) {
                if (_j == j) {
                    col = 1;
                    continue;
                }
                arr[_i - str][_j - col] = this->data[_i][_j];
            }
            col = 0;
        }
        Matrix ans(*arr, this->n - 1, this->m - 1);
        return ans;
    }

    double determinant() {
        if (!this->is_square()) {
            std::cout << "WARNING! The determinant is defined only for square matrices." << '\n';
            return nan("1");
        }
        if (this->n == 1 && this->m == 1) {
            return this->data[0][0];
        }
        double ans = 0;
        for (int i = 0; i < this->m; i++) {
            ans += (this->algebraic_addition(0, i)) * this->data[0][i];
        }
        return ans;
    }

};


#endif //APPROXIMATION_MATRIX_H
