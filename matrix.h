#ifndef APPROXIMATION_MATRIX_H
#define APPROXIMATION_MATRIX_H
#include <cmath>

template <typename T>
class Matrix;

template <typename T>
std::ostream& operator << (std::ostream &out, const Matrix<T> &matrix) {
    for (int i = 0; i < matrix.n; i++) {
        for (int j = 0; j < matrix.m; j++) {
            std::cout << matrix.data[i][j] << ' ';
        }
        std::cout << '\n';
    }
    return out;
}

template <typename T>
std::istream& operator >> (std::istream &in, Matrix<T> &matrix) {
    in >> matrix.n >> matrix.m;
    matrix.data = new T*[matrix.n];
    for (int i = 0; i < matrix.n; i++) {
        matrix.data[i] = new T[matrix.m];
        for (int j = 0; j < matrix.m; j++) {
            in >> matrix.data[i][j];
        }
    }
    return in;
}

template <typename T>
class Matrix {
public:
    T** data;

private:
    int n, m;

public:

    Matrix() { }

    Matrix(T* arr, int n, int m) {
        this->n = n;
        this->m = m;
        this->data = new T*[n];
        for (int i = 0; i < n; i++) {
            this->data[i] = new T[m];
            for (int j = 0; j < m; j++) {
                this->data[i][j] = arr[i * m + j];
            }
        }
    }

    Matrix(const Matrix &other) {
        this->n = other.n;
        this->m = other.m;
        this->data = new T*[other.n];
        for (int i = 0; i < other.n; i++) {
            this->data[i] = new T[other.m];
            for (int j = 0; j < other.m; j++) {
                this->data[i][j] = other.data[i][j];
            }
        }
    }

    ~Matrix() {
        for (int i = 0; i < n; i++){
            delete[] data[i];
        }
        delete[] data;
    }

    Matrix & operator = (const Matrix &other) {
        if (&other == this) {
            return *this;
        }
        if (this->data != nullptr) {
            for (int i = 0; i < n; i++){
                delete[] data[i];
            }
            delete[] data;
            data = nullptr;
        }
        this->n = other.n;
        this->m = other.m;
        this->data = new T*[other.n];
        for (int i = 0; i < other.n; i++) {
            this->data[i] = new T[other.m];
            for (int j = 0; j < other.m; j++) {
                this->data[i][j] = other.data[i][j];
            }
        }
        return *this;
    }

    Matrix operator + (const Matrix &other) {
        if (this->n != other.n || this->m != other.m) {
            std::cout << "Summ is defined only for same dimentioned matrixes." << '\n';
        }
        T arr[this->n][this->m];
        for (int i = 0; i < this->n; i++) {
            for (int j = 0; j < this->m; j++) {
                arr[i][j] = this->data[i][j] + other.data[i][j];
            }
        }
        Matrix ans(*arr, this->n, this->m);
        return ans;
    }

    Matrix operator - (const Matrix &other) {
        return (other * -1) + *this;
    }

    Matrix operator * (const Matrix &other) {
        if (this->m != other.n) {
            std::cout << "WARNING! The row length of the first matrix is not equal to the column length of the second one." << '\n';
        }
        T arr[this->n][other.m];
        for (int i = 0; i < this->n; i++) {
            for (int j = 0; j < other.m; j++) {
                arr[i][j] = 0;
                for (int k = 0; k < this->m; k++) {
                    arr[i][j] += this->data[i][k] * other.data[k][j];
                }
            }
        }
        Matrix ans(*arr, this->n, other.m);
        return ans;
    }

    Matrix operator * (const T num) const {
        T arr[this->n][this->m];
        for (int i = 0; i < this->n; i++) {
            for (int j = 0; j < this->m; j++) {
                arr[i][j] = this->data[i][j] * num;
            }
        }
        Matrix ans(*arr, this->n, this->m);
        return ans;
    }

    Matrix operator | (const Matrix &other) {
        if (this->n != other.n) {
            std::cout << "To merge right both matrixes have to have same column length." << '\n';
        }
        T arr[this->n][this->m + other.m];
        for (int i = 0; i < this->n; i++) {
            for (int j = 0; j < this->m; j++) {
                arr[i][j] = this->data[i][j];
            }
        }
        for (int i = 0; i < other.n; i++) {
            for (int j = 0; j < other.m; j++) {
                arr[i][j + this->m] = other.data[i][j];
            }
        }
        Matrix ans(*arr, this->n, this->m + other.m);
        return ans;
    }

    Matrix operator ^ (const Matrix &other) {
        if (this->m != other.m) {
            std::cout << "To merge bottom both matrixes have to have same row length." << '\n';
        }
        T arr[this->n + other.n][this->m];
        for (int i = 0; i < this->n; i++) {
            for (int j = 0; j < this->m; j++) {
                arr[i][j] = this->data[i][j];
            }
        }
        for (int i = 0; i < other.n; i++) {
            for (int j = 0; j < other.m; j++) {
                arr[i + this->n][j] = other.data[i][j];
            }
        }
        Matrix ans(*arr, this->n + other.n, this->m);
        return ans;
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
        T ans[n][m];
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                ans[i][j] = this->data[start_str + i][start_col + j];
            }
        }
        Matrix submatrix(*ans, n, m);
        return submatrix;
    }

    T algebraic_addition(int i, int j) {
        T sign = -1;
        if ((i + j) % 2 == 0) {
            sign = 1;
        }
        return (this->minor(i, j).determinant()) * sign;
    }

    Matrix inverse() {
        if (!this->is_square()) {
            std::cout << "WARNING! The inverse matrix is defined only for square matrices." << '\n';
            Matrix ans;
            return ans;
        }
        T arr[this->n][this->m];
        T det = this->determinant();
        for (int i = 0; i < this->n; i++) {
            for (int j = 0; j < this->m; j++) {
                arr[i][j] = this->algebraic_addition(i, j) / det;
            }
        }
        Matrix ans(*arr, this->n, this->m);
        return ans;
    }

    Matrix minor(int i, int j) {
        T arr[this->n - 1][this->m - 1];
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

    T determinant() {
        if (!this->is_square()) {
            std::cout << "WARNING! The determinant is defined only for square matrices." << '\n';
            return nan("1");
        }
        if (this->n == 1 && this->m == 1) {
            return this->data[0][0];
        }
        T ans = 0;
        for (int i = 0; i < this->m; i++) {
            ans += (this->algebraic_addition(0, i)) * this->data[0][i];
        }
        return ans;
    }

    friend std::ostream& operator << <T>(std::ostream &out, const Matrix &matrix);
    friend std::istream& operator >> <T>(std::istream &in, Matrix &matrix);

};

template <typename T>
Matrix<T> operator * (const double num, const Matrix<T> &M) {
     return M * num;
}

#endif //APPROXIMATION_MATRIX_H=
