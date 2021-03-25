#ifndef APPROXIMATION_MATRIX_H
#define APPROXIMATION_MATRIX_H
#include <exception>
#include <cmath>

class Matrix_exception: public std::exception{
public:
    explicit Matrix_exception(std::string r): reason_{std::move(r)}{}

    char const* what() const noexcept {
        return reason_.c_str();
    }
private:
    std::string reason_;
};

template <typename T>
class Matrix;

template <typename T>
std::ostream& operator << (std::ostream &out, const Matrix<T> &matrix) {
    for (int i = 0; i < matrix.n; i++) {
        for (int j = 0; j < matrix.m; j++) {
            out << matrix.data[i][j] << ' ';
        }
        out << '\n';
    }
    return out;
}

template <typename T>
std::istream& operator >> (std::istream &in, Matrix<T> &matrix) {
    in >> matrix.n >> matrix.m;
    if (matrix.n < 1 || matrix.m < 1) {
        throw Matrix_exception("ERROR! Cannot create matrix with negative dimentions");
    }
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

protected:
    int n, m;

public:

    Matrix<T> () {}

    Matrix<T> (int n, int m, T filler) {
        if (n < 1 || m < 1) {
            throw Matrix_exception("Cannot create matrix with negative dimentions");
        }
        this->n = n;
        this->m = m;
        this->data = new T*[n];
        for (int i = 0; i < n; i++) {
            this->data[i] = new T[m];
            for (int j = 0; j < m; j++) {
                this->data[i][j] = filler;
            }
        }
    }

    Matrix<T> (int n, int m) {
        if (n < 1 || m < 1) {
            throw Matrix_exception("Cannot create matrix with dispositive dimentions");
        }
        this->n = n;
        this->m = m;
        this->data = new T* [n];
        for (int i = 0; i < n; i++) {
            this->data[i] = new T[m];
        }
        for (int i = 0; i < n; i++){
            for (int j = 0; j < m; j++){
                data[i][j] = 0;
            }
        }
    }

    Matrix<T> (T* arr, int n, int m) {
        if (n < 1 || m < 1) {
            throw Matrix_exception("Cannot create matrix with dispositive dimentions");
        }
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

    Matrix<T> (Matrix <T> const &other) {
        int n = other.get_dimensions().first, m = other.get_dimensions().second;
        this->n = n;
        this->m = m;
        this->data = new T*[n];
        for (int i = 0; i < n; i++) {
            this->data[i] = new T[m];
            for (int j = 0; j < m; j++) {
                this->data[i][j] = other.data[i][j];
            }
        }
    }

    Matrix<T> (Matrix<T> &&other) {
        this->n = other.getN();
        this->m = other.getM();
        this->data = other.data;
        other.data = nullptr;
    }

    ~Matrix() {
        if (data != nullptr) {
            for (int i = 0; i < n; i++){
                delete[] data[i];
            }
        delete[] data;
        }
    }

    Matrix<T> & operator = (Matrix <T> &other) {
        if (&other == this) {
            return *this;
        }
        Matrix<T> ans(this->n, this->m);

        for (int i = 0; i < n; i++){
            for (int j = 0; j < m; j++){
                ans(i, j) = other(i, j);
            }
        }
        return ans;
    }

    Matrix<T> & operator = (Matrix <T> &&other) {
        if (this != &other) {
            this->n = other.getN();
            this->m = other.getM();
            this->data = other.data;
            other.data = nullptr;
        }
        return *this;
    }

    void resize(int n, int m, T filler) {
        if (n < 1 || m < 1) {
            throw Matrix_exception("Cannot create matrix with negative dimentions");
        }
        for (int i = 0; i < this->n; i++){
            delete[] this->data[i];
        }
        delete[] this->data;
        this->n = n;
        this->m = m;
        this->data = new T*[n];
        for (int i = 0; i < n; i++) {
            this->data[i] = new T[m];
            for (int j = 0; j < m; j++) {
                this->data[i][j] = filler;
            }
        }
    }

    void resize(int n, int m) {
        if (n < 1 || m < 1) {
            throw Matrix_exception("Cannot create matrix with negative dimentions");
        }
        for (int i = 0; i < this->n; i++){
            delete[] this->data[i];
        }
        delete[] this->data;
        this->n = n;
        this->m = m;
        this->data = new T*[n];
        for (int i = 0; i < n; i++) {
            this->data[i] = new T[m];
        }
    }

    Matrix<T> operator + (Matrix<T> const &other) const {
        int n = other.getN(), m = other.getM();
        if (!(this->n == n && this->m == m)) {
            throw Matrix_exception("Sum is defined only for same dimensioned matrices.");
        }
        Matrix<T> ans(this->n, this->m);
        for (int i = 0; i < this->n; i++) {
            for (int j = 0; j < this->m; j++) {
                ans(i, j) = this->data[i][j] + other(i, j);
            }
        }
        return ans;
    }

    Matrix<T> operator - (Matrix const &other) const {
        return (other * -1) + *this;
    }

    Matrix<T> operator * (Matrix const &other) const {

        Matrix<T> ans(n, other.getM());
        if (m != other.getN()) {
            throw Matrix_exception("WARNING! The row length of the first matrix is not equal to the column length of the second one.");
        }
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < other.getM(); j++) {
                ans(i, j) = 0;
                for (int k = 0; k < m; k++) {
                    ans(i, j) += (data[i][k] * other(k, j));
                }
            }
        }
        return ans;
    }

    Matrix<T> operator * (double num) const {
        Matrix<T> ans(this->n, this->m);
        for (int i = 0; i < this->n; i++) {
            for (int j = 0; j < this->m; j++) {
                ans(i, j) = this->data[i][j] * num;
            }
        }
        return ans;
    }

    Matrix<T> operator / (double num) const {
        return *this * (1.0 / num);
    }

    Matrix<T> operator | (Matrix const &other) const {
        int n = other.get_dimensions().first, m = other.get_dimensions().second;
        if(this->n != n) {
            throw Matrix_exception("To merge right both matrixes have to have same column length.");
        }
        T arr[this->n][this->m + m];
        for (int i = 0; i < this->n; i++) {
            for (int j = 0; j < this->m; j++) {
                arr[i][j] = this->data[i][j];
            }
        }
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                arr[i][j + this->m] = other.data[i][j];
            }
        }
        Matrix ans(*arr, this->n, this->m + m);
        return ans;
    }

    Matrix<T> operator ^ (Matrix const &other) const {
        int n = other.get_dimensions().first, m = other.get_dimensions().second;
        if (this->m != m) {
            throw Matrix_exception("To merge bottom both matrices have to have same row length.");
        }
        T arr[this->n + n][this->m];
        for (int i = 0; i < this->n; i++) {
            for (int j = 0; j < this->m; j++) {
                arr[i][j] = this->data[i][j];
            }
        }
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                arr[i + this->n][j] = other.data[i][j];
            }
        }
        Matrix ans(*arr, this->n + n, this->m);
        return ans;
    }

    T& operator()(int i, int j) const {
        if (i < 0 || i >= this->n || j < 0 || j >=  this->m) {
            throw Matrix_exception("WARNING! Array overflow");
        }
        return this->data[i][j];
    }

    void swap(int i, int j) {
        if (!((i < this->n) && (j < this->n))) {
            throw Matrix_exception("WARNING! Wrong index. Swap error.");
        }
        for (int c = 0; c < this->m; c++){
            auto tmp = data[j][c];
            data[j][c] = data[i][c];
            data[i][c] = tmp;
        }
    }

    std::pair <int, int> get_dimensions() const {
        return std::make_pair(this->n, this->m);
    }

    int getN() const {
        return this->n;
    }

    int getM() const {
        return this->m;
    }

    bool is_square() const {
        if (this->n == this->m) {
            return true;
        }
        return false;
    }

    Matrix<T> get_submatrix(int start_str, int start_col, int n, int m) const {
        if (start_str < 0 || start_col < 0 || start_str + n >= this->n || start_col + m >= this->m) {
            throw Matrix_exception("ERROR! Wrong dimentions");
        }
        T ans[n][m];
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                ans[i][j] = this->data[start_str + i][start_col + j];
            }
        }
        Matrix submatrix(*ans, n, m);
        return submatrix;
    }

    T algebraic_addition(int i, int j) const {
        if (i < 0 || j < 0 || i >= this->n || j >= this->m) {
            throw Matrix_exception("ERROR! Array overflow");
        }
        if (this->n == 1) {
            return 1;
        }
        double sign = -1;
        if ((i + j) % 2 == 0) {
            sign = 1;
        }
        return (this->minor(i, j).determinant()) * sign;
    }

    Matrix<T> transpose() const{
        Matrix <T> ans(*this);
        for (int i = 0; i < this->n; i++) {
            for (int j = 0; j < i; j++) {
                T a1 = ans(i, j), a2 = ans(j, i);
                ans(i, j) = a2;
                ans(j, i) = a1;
            }
        }
        return ans;
    }

    Matrix<T> inverse() const {
        T det = this->determinant();
        if (!(this->is_square())) {
            throw Matrix_exception("WARNING! The inverse matrix is defined only for square matrices.");
        }
        if (det == 0) {
            throw Matrix_exception("WARNING! The inverse matrix is defined only for matrices with determinant not equal to 0.");
        }
        T arr[this->n][this->m];
        for (int i = 0; i < this->n; i++) {
            for (int j = 0; j < this->m; j++) {
                arr[i][j] = this->algebraic_addition(i, j) / det;
            }
        }
        Matrix ans(*arr, this->n, this->m);
        return ans.transpose();
    }

    Matrix<T> minor(int i, int j) const {
        if (i < 0 || j < 0 || i >= this->n || j >= this->m) {
            throw Matrix_exception("ERROR! Array overflow");
        }
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

    Matrix<T> makeL() const {
        Matrix<T> L;
        if (n != m) throw Matrix_exception("Warning! LU normally work only for square matrix");
        for (auto i = 0; i < m; i++) {
            for (auto j = i + 1; j < n; j++){
                L(i, j) = data[i][j];
            }
        }
        return L;
    }

    T norm() const {
        T sum = 0;
        for(size_t i = 0; i < m; ++i){
            sum += data[1][i] * data[1][i];
        }
        return sqrt(sum);
    }

    T determinant() const {
        if (!(this->is_square())) {
            throw Matrix_exception("WARNING! The determinant is defined only for square matrices.");
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
Matrix<T> operator * (double num, Matrix<T> const &M) {
     return M * num;
}

template <typename T>
T norm(const Matrix<T>& a){
    T sum = 0;
    for(auto i = 0; i < a.getN(); ++i){
        sum += a(1, i) * a(1, i);
    }
    return sqrt(sum);
}

template <typename T>
class Matrix_U: public Matrix<T> {
    public:
        Matrix_U <T> (Matrix<T> A) {
            this->n = A.getN();
            this->m = A.getM();
            this->data = new T*[this->n];
            for (int i = 0; i < this->n; i++) {
                this->data[i] = new T[this->m];
            }
            for (size_t i = 0; i < A.getN(); i++){
                for (size_t j = 0; j < A.getM(); j++){
                    if (i > j) this->data[i][j] = A(i, j);
                    else this->data[i][j] = 0;
                }
            }
        }
    };

template <typename T>
class Matrix_L: public Matrix<T> {
public:
    Matrix_L <T> (Matrix<T> A) {
        this->n = A.getN();
        this->m = A.getM();
        this->data = new T*[this->n];
        for (int i = 0; i < this->n; i++) {
            this->data[i] = new T[this->m];
        }
        for (size_t i = 0; i < A.getN(); i++){
            for (size_t j = 0; j < A.getM(); j++){
                if (i <= j) this->data[i][j] = A(i, j);
                else this->data[i][j] = 0;
            }
        }
    }
};


#endif //APPROXIMATION_MATRIX_H=
