#ifndef MOLDYN_MATRIX_HPP
#define MOLDYN_MATRIX_HPP

#include <algorithm>
#include <cmath>
#include <initializer_list>
#include <vector>
#include <iostream>
#include <functional>
#include <ostream>
#include <numeric>
#include <string>
#include <memory>

template <typename T, size_t M, size_t N>
class Matrix;

template <typename T, size_t N>
using CVec = Matrix<T, N, 1>;
template <typename T, size_t N>
using RVec = Matrix<T, 1, N>;

template <typename T, size_t M, size_t N>
class Matrix {
    public:
        std::shared_ptr<std::vector<T>> data = std::make_shared<std::vector<T>>(M * N);
        size_t rows = M;
        size_t cols = N;
        size_t stride = 1;
        size_t offset = 0;
        Matrix() = default;
        Matrix(T val) { std::fill(data->begin(), data->end(), val); }
        Matrix(std::initializer_list<T> list) { std::copy(list.begin(), list.end(), data->begin()); }
        Matrix(const Matrix<T, M, N>& mat) : data(mat.data) {}

        template <typename V>
        Matrix<V, M, N> map(std::function<V(T)> f) const {
            Matrix<V, M, N> result;
            for (size_t i = 0; i < M; i++) {
                for (size_t j = 0; j < N; j++) {
                    std::cout << result(i, j) << " " << (*this)(i, j) << " " << f((*this)(i, j)) << " " << i << " " << j << " " << M << " " << N << std::endl;
                    result(i, j) = f((*this)(i, j));
                }
            }
            return result;
        }
        template <typename U, typename V>
        Matrix<V, M, N> map2(Matrix<U, M, N> m2, std::function<V(T, U)> f) const {
            Matrix<V, M, N> result;
            for (size_t i = 0; i < M; i++) {
                for (size_t j = 0; j < N; j++) {
                    result(i, j) = f((*this)(i, j), m2(i, j));
                }
            }
            return result;
        }
        template <typename V>
        V accumulate(V init, std::function<V(V, T)> f) const {
            for (size_t i = 0; i < M; i++) {
                for (size_t j = 0; j < N; j++) {
                    init = f(init, (*this)(i, j));
                }
            }
            return init;
        }
        T reduce(std::function<T(T, T)> f) const {
            T result = this(0, 0);
            for (size_t i = 1; i < M; i++) {
                for (size_t j = 0; j < N; j++) {
                    if (i != 0 && j != 0) { result = f(result, (*this)(i, j)); }
                }
            }
            return result;
        }

        Matrix<T, M, N> operator+(const Matrix<T, M, N>& mat) const {
            return map2<T, T>(mat, std::plus<T>());
        }
        Matrix<T, M, N> operator-(const Matrix<T, M, N>& mat) const {
            return map2<T, T>(mat, std::minus<T>());
        }
        Matrix<T, M, N> operator*(const Matrix<T, M, N>& mat) const {
            return map2<T, T>(mat, std::multiplies<T>());
        }
        Matrix<T, M, N> operator/(const Matrix<T, M, N>& mat) const {
            return map2<T, T>(mat, std::divides<T>());
        }
        Matrix<T, M, N> operator+(const T& val) const {
            return map<T>(std::bind2nd(std::plus<T>(), val));
        }
        Matrix<T, M, N> operator-(const T& val) const {
            return map<T>(std::bind2nd(std::minus<T>(), val));
        }
        Matrix<T, M, N> operator*(const T& val) const {
            return map<T>(std::bind2nd(std::multiplies<T>(), val));
        }
        Matrix<T, M, N> operator/(const T& val) const {
            return map<T>(std::bind2nd(std::divides<T>(), val));
        }
        Matrix<T, M, N> operator-() const {
            return map<T>(std::negate<T>());
        }

        Matrix<T, M, N> &operator=(const Matrix<T, M, N>& mat) {
            for (size_t i = 0; i < M; i++) {
                for (size_t j = 0; j < N; j++) {
                    (*this)(i, j) = mat(i, j);
                }
            }
            return *this;
        }
        Matrix<T, M, N> &operator+=(const Matrix<T, M, N>& mat) {
            return *this = *this + mat;
        }
        Matrix<T, M, N> &operator-=(const Matrix<T, M, N>& mat) {
            return *this = *this - mat;
        }
        Matrix<T, M, N> &operator*=(const Matrix<T, M, N>& mat) {
            return *this = *this * mat;
        }
        Matrix<T, M, N> &operator/=(const Matrix<T, M, N>& mat) {
            return *this = *this / mat;
        }
        Matrix<T, M, N> &operator+=(const T& val) {
            return *this = *this + val;
        }
        Matrix<T, M, N> &operator-=(const T& val) {
            return *this = *this - val;
        }
        Matrix<T, M, N> &operator*=(const T& val) const {
            return *this = *this * val;
        }
        Matrix<T, M, N> &operator/=(const T& val) const {
            return *this = *this / val;
        }

        template<size_t O>
        Matrix<T, M, O> mm(const Matrix<T, N, O>& mat) const {
            Matrix<T, M, O> result(0.);
            for (int j = 0; j < N; ++j) {
                for (int i = 0; i < M; ++i) {
                    for (int k = 0; k < O; ++k) {
                        result(i, k) += (*this)(i, j) * mat(j, k);
                    }
                }
            }
            return result;
        }
        Matrix<T, N, M> transpose() const {
            Matrix<T, N, M> result;
            for (int i = 0; i < M; ++i) {
                for (int j = 0; j < N; ++j) {
                    result(j, i) = (*this)(i, j);
                }
            }
            return result;
        }

        Matrix<bool, M, N> operator==(const Matrix<T, M, N>& m2) const {
            return map2(m2, [](T a, T b) { return a == b; });
        }
        Matrix<bool, M, N> operator!=(const Matrix<T, M, N>& m2) const {
            return map2(m2, [](T a, T b) { return a != b; });
        }
        Matrix<bool, M, N> operator<(const Matrix<T, M, N>& m2) const {
            return map2(m2, [](T a, T b) { return a < b; });
        }
        Matrix<bool, M, N> operator>(const Matrix<T, M, N>& m2) const {
            return map2(m2, [](T a, T b) { return a > b; });
        }
        Matrix<bool, M, N> operator<=(const Matrix<T, M, N>& m2) const {
            return map2(m2, [](T a, T b) { return a <= b; });
        }
        Matrix<bool, M, N> operator>=(const Matrix<T, M, N>& m2) const {
            return map2(m2, [](T a, T b) { return a >= b; });
        }

        T& operator()(size_t i) {
            return (*data)[i * stride + offset];
        }
        T& operator()(size_t i, size_t j) {
            std::cout << "operator() " << i << " " << j << " " << stride << " " << offset << " " << (i * M + j) * stride + offset << " " << (*data)[(i * M + j) * stride + offset] << std::endl;
            return (*data)[(i * M + j) * stride + offset];
        }
        const T& operator()(size_t i, size_t j) const {
            return (*data)[(i * M + j) * stride + offset];
        }
        RVec<T, N> row(size_t i) const {
            RVec<T, N> result;
            result.data = data;
            result.offset = i * M;
            result.stride = 1;
            return result;
        }
        CVec<T, M> col(size_t j) const {
            CVec<T, M> result;
            result.data = data;
            result.offset = j;
            result.stride = 1;
            return result;
        }

        T sum() const {
            return accumulate(0, std::plus<T>());
        }
        T prod() const {
            return accumulate(1, std::multiplies<T>());
        }
        T min() const {
            return reduce([](T a, T b) { return a < b ? a : b; });
        }
        T max() const {
            return reduce([](T a, T b) { return a > b ? a : b; });
        }
};

template <typename T, size_t M, size_t N>
Matrix<T, M, N> operator+(const T& val, const Matrix<T, M, N> mat) {
    return mat + val;
}
template <typename T, size_t M, size_t N>
Matrix<T, M, N> operator-(const T& val, const Matrix<T, M, N> mat) {
    return (-mat) + val;
}
template <typename T, size_t M, size_t N>
Matrix<T, M, N> operator*(const T& val, const Matrix<T, M, N> mat) {
    return mat * val;
}
template <typename T, size_t M, size_t N>
Matrix<T, M, N> operator/(const T& val, const Matrix<T, M, N> mat) {
    return mat.template map<T>(std::bind1st(std::divides<T>(), val));
}

template <typename T, size_t M, size_t N>
std::ostream &operator<<(std::ostream& os, const Matrix<T, M, N> mat) {
    if (N == 0 || M == 0) { os << "[ ]"; return os; }
    for (size_t i = 0; i < M - 1; ++i) {
        os << "[ ";
        for (size_t j = 0; j < N - 1; ++j) {
            os << mat(i, j) << ", ";
        }
        os << mat(i, N - 1) << " ]\n";
    }
    os << "[ " << mat(M - 1, N - 1) << " ]";
    return os;
}

#endif
