#ifndef APPROXIMATION_MATRIX_H
#define APPROXIMATION_MATRIX_H
#include <exception>

class Polynom_exception: public std::exception{
public:
    explicit Polynom_exception(std::string r): reason_{std::move(r)}{}

    char const* what() const noexcept {
        return reason_.c_str();
    }
private:
    std::string reason_;
};

const double EPS = 0.0000000001;
template <typename T>
double my_abs(T num) {
    if (num > 0) {
        return num;
    }
    return num * (T)-1;
}

template <typename T>
class Polynom;

template <typename T>
std::ostream& operator << (std::ostream &out, const Polynom<T> &polynom) {
    bool flag = true, flag1 = false;
    if (polynom.data[0] != 0) {
        out << polynom.data[0];
        flag1 = true;
        flag = false;
    }
    for (int i = 1; i < polynom.n + 1; i++) {
        if (my_abs(polynom.data[i]) < EPS) {
            continue;
        }
        if (polynom.data[i] < 0) {
            if (!flag) {
                out << " - ";
                flag1 = true;
            } else {
                out << "-";
            }
            out << (-1) * polynom.data[i] << "x^" << i;
            flag = false;
        } else {
            if (!flag) {
                out << " + ";
                flag1 = true;
            }
            out << polynom.data[i] << "x^" << i;
            flag = false;
        }
    }
    if (flag) {
        out << 0;
    }
    return out;
}

template <typename T>
std::istream& operator >> (std::istream &in, Polynom<T> &polynom) {
    in >> polynom.n;
    if (polynom.n < 0) {
        throw Polynom_exception("ERROR! Power of polynom should not be negative");
    }
    polynom.data = new T[polynom.n + 1];
    for (int i = 0; i < polynom.n + 1; i++) {
        in >> polynom.data[i];
    }
    return in;
}

template <typename T>
class Polynom {
public:
    T* data;

private:
    int n;

public:

    Polynom() {}

    Polynom <T> (T *arr, int n) {
        if (n < 0) {
            throw Polynom_exception("ERROR! Power of polynom should not be negative");
        }
        this->data = new T[n + 1];
        this->n = n;
        for (int i = 0; i < n+1; i++) {
            this->data[i] = arr[i];
        }
    }

    Polynom <T> (int n, T filler) {
        if (n < 0) {
            throw Polynom_exception("ERROR! Power of polynom should not be negative");
        }
        this->data = new T[n + 1];
        this->n = n;
        for (int i = 0; i < n + 1; i++) {
            this->data[i] = filler;
        }
    }

    Polynom <T> (int n) {
        if (n < 0) {
            throw Polynom_exception("ERROR! Power of polynom should not be negative");
        }
        this->data = new T[n + 1];
        this->n = n;
    }

    Polynom <T> (Polynom<T> const &other) {
        int n = other.get_pow();
        this->n = n;
        for (int i = 0; i < n + 1; i++) {
            this->data[i] = other.data[i];
        }
    }

    Polynom (Polynom<T> &&other) {
        this->n = other.get_pow();
        this->data = other.data;
        other.data = nullptr;
    }

    ~Polynom() {
        delete[] data;
    }

    int get_pow() const {
        return this->n;
    }

    int get_real_pow() const{
        int ans = n;
        for (int i = 0; i < n + 1; i++) {
            if (this->data[n - i] == 0) {
                ans--;
            } else {
                break;
            }
        }
        return ans;
    }

    T& operator[](int i) {
        if ((i < 0) || (i >= this->n + 1)) {
            throw Polynom_exception("ERROR! Array overflow");
        }
        return this->data[i];
    }

    Polynom<T> & operator = (Polynom <T> const &other) {
        if (&other == this) {
            return *this;
        }
        if (this->data != nullptr) {
            delete[] data;
            data = nullptr;
        }
        int n = other.get_pow();
        this->n = n;
        this->data = new T[n + 1];
        for (int i = 0; i < n + 1; i++) {
            this->data[i] = other.data[i];
        }
        return *this;
    }

    Polynom<T> & operator = (Polynom <T> const &&other) {
        if (this != &other) {
            this->n = other.get_pow();
            this->data = other.data;
            other.data = nullptr;
        }
        return *this;
    }

    Polynom<T> operator + (Polynom <T> const &other) const {
        int n = other.get_pow();
        T arr[std::max(this->n + 1, n + 1)];
        for (int i = 0; i < std::min(this->n, n) + 1; i++) {
            arr[i] = this->data[i] + other.data[i];
        }
        if (this->n > n) {
            for (int i = n + 1; i < this->n + 1; i++) {
                arr[i] = this->data[i];
            }
        } else {
            for (int i = this->n + 1; i < n + 1; i++) {
                arr[i] = other.data[i];
            }
        }
        Polynom <T> ans(arr, std::max(this->n, n));
        return ans;
    }

    Polynom <T> operator + (T num) const {
        Polynom <T> temp(0, num);
        return *this + temp;
    }

    Polynom<T> operator - (T num) const {
        return *this + (num * (T)-1);
    }

    Polynom<T> operator - (Polynom <T> const &other) const {
        return ((other * (-1)) + *this);
    }

    Polynom<T> operator * (double num) const {
        T arr[this->n + 1];
        for (int i = 0; i < this->n + 1; i++) {
            arr[i] = this->data[i] * num;
        }
        Polynom <T> ans(arr, this->n);
        return ans;
    }

    Polynom<T> operator * (Polynom <T> const &other) const {
        int n = other.get_pow();
        T arr[this->n + n + 1];
        for (int i = 0; i < this->n + n + 1; i++) {
            arr[i] = 0;
        }
        for (int i = 0; i < this->n + 1; i++) {
            for (int j = 0; j < n + 1; j++) {
                arr[i + j] += this->data[i] * other.data[j];
            }
        }
        Polynom <T> ans(arr, this->n + n);
        return ans;
    }

    Polynom<T> operator / (T num) const {
        if (num == 0) {
            throw Polynom_exception("ERROR! Division by zero");
        }
        return *this * ((T)1 / num);
    }

    void resize(int n, T filler) {
        if (n < 0) {
            throw Polynom_exception("ERROR! Power of polynom should not be negative");
        }
        delete[] this->data;
        this->n = n;
        this->data = new T[n + 1];
        for (int i = 0; i < n + 1; i++) {
            this->data[i] = filler;
        }
    }

    void resize(int n) {
        if (n < 0) {
            throw Polynom_exception("ERROR! Power of polynom should not be negative");
        }
        delete[] this->data;
        this->n = n;
        this->data = new T[n + 1];
    }

    void clear_end() {
        int length = 0;
        for (int i = this->n; i >= 0; i--) {
            if (my_abs(this->data[i]) < EPS) {
                length++;
            } else {
                break;
            }
        }
        if (length == 0) {
            return;
        }
        if (n - length + 1 == 0) {
            Polynom ans(0, (T)0);
            *this = ans;
            return;
        }
        T arr[n + 1 - length];
        for (int i = 0; i < n + 1 - length; i++) {
            arr[i] = this->data[i];
        }
        Polynom ans(arr, n - length);
        *this = ans;
    }

    bool is_zero() const {
        if ((this->get_real_pow() == 0) && (my_abs(this->data[0]) < EPS)) {
            return true;
        }
        return false;
    }

    friend std::ostream& operator << <T>(std::ostream &out, const Polynom<T> &polynom);
    friend std::istream& operator >> <T>(std::istream &in, Polynom<T> &polynom);

};

template <typename T>
Polynom<T> operator + (double num, Polynom<T> const &P) {
     return P + num;
}

template <typename T>
Polynom<T> operator - (double num, Polynom<T> const &P) {
     return (P * (-1)) + num;
}

template <typename T>
Polynom<T> operator * (double num, Polynom<T> const &M) {
     return M * num;
}

#endif //APPROXIMATION_MATRIX_H=
