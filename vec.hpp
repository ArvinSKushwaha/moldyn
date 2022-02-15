#ifndef MOLDYN_VEC_HPP
#define MOLDYN_VEC_HPP

#include <algorithm>
#include <cmath>
#include <initializer_list>
#include <array>
#include <iostream>
#include <functional>

template <typename T, size_t N>
class Vec {
    private:
        std::array<T, N> data;
    public:
        // Constructors
        Vec() = default;
        Vec(T);
        Vec(std::initializer_list<T>);
        Vec(const Vec<T, N>&);

        // Apply closure
        template <typename U>
        Vec<U, N> map(std::function<U(T)>) const;
        template <typename U>
        Vec<U, N> map2(Vec<T, N>, std::function<U(T, T)>) const;

        // Math Operators
        Vec<T, N> operator+(const Vec<T, N>&) const;
        Vec<T, N> operator-(const Vec<T, N>&) const;
        Vec<T, N> operator*(const Vec<T, N>&) const;
        Vec<T, N> operator/(const Vec<T, N>&) const;
        Vec<T, N> operator+(const T&) const;
        Vec<T, N> operator-(const T&) const;
        Vec<T, N> operator*(const T&) const;
        Vec<T, N> operator/(const T&) const;
        Vec<T, N> operator-() const;

        // Assignment Operators
        Vec<T, N>& operator=(const Vec<T, N>&);
        Vec<T, N>& operator+=(const Vec<T, N>&);
        Vec<T, N>& operator-=(const Vec<T, N>&);
        Vec<T, N>& operator*=(const Vec<T, N>&);
        Vec<T, N>& operator/=(const Vec<T, N>&);
        Vec<T, N>& operator+=(const T&);
        Vec<T, N>& operator-=(const T&);
        Vec<T, N>& operator*=(const T&);
        Vec<T, N>& operator/=(const T&);

        // Comparison Operators
        Vec<bool, N> operator==(const Vec<T, N>&) const;
        Vec<bool, N> operator!=(const Vec<T, N>&) const;
        Vec<bool, N> operator<(const Vec<T, N>&) const;
        Vec<bool, N> operator>(const Vec<T, N>&) const;
        Vec<bool, N> operator<=(const Vec<T, N>&) const;
        Vec<bool, N> operator>=(const Vec<T, N>&) const;

        // Access Operators
        T& operator[](size_t);
        const T& operator[](size_t) const;

        // Other Functions
        T mag() const;
        T mag2() const;
        Vec<T, N> normalize(float=1e-5) const;
        Vec<T, N> normalizeFast() const;
        T dot(const Vec<T, N>&) const;

        // Reduction Functions
        T sum() const;
        T prod() const;
        T min() const;
        T max() const;
};

// Reverse Math Operators
template <typename T, size_t N>
Vec<T, N> operator+(const T&, const Vec<T, N>&);
template <typename T, size_t N>
Vec<T, N> operator-(const T&, const Vec<T, N>&);
template <typename T, size_t N>
Vec<T, N> operator*(const T&, const Vec<T, N>&);
template <typename T, size_t N>
Vec<T, N> operator/(const T&, const Vec<T, N>&);

// Cross Product for 3D
template <typename T>
Vec<T, 3> cross(const Vec<T, 3>&, const Vec<T, 3>&);

// IO
template <typename T, size_t N>
std::ostream &operator<<( std::ostream &, const Vec<T, N> &);

/**
 * Construct a vector filled with the given value.
 */
template<typename T, size_t N>
Vec<T, N>::Vec(T val)
{
    std::fill(data.begin(), data.end(), val);
}

/**
 * Construct a vector from a list of values.
 */
template<typename T, size_t N>
Vec<T, N>::Vec(std::initializer_list<T> il)
{
    std::copy(il.begin(), il.end(), data.begin());
}

/**
 * Make a copy of the given vector.
 */
template<typename T, size_t N>
Vec<T, N>::Vec(const Vec<T, N> &v)
{
    std::copy(v.data.begin(), v.data.end(), data.begin());
}

/**
 * Apply the given function to each element of the vector.
 * The function must take a single argument of type T.
 * The function must return a value of type U.
 */
template<typename T, size_t N>
template<typename U>
Vec<U, N> Vec<T, N>::map(std::function<U(T)> f) const
{
    Vec<U, N> result;
    for (size_t i = 0; i < N; i++)
        result[i] = f(data[i]);
    return result;
}

/**
 * Apply the given function to each element of the vector.
 * The function must take two arguments of type T.
 * The function must return a value of type U.
 */
template<typename T, size_t N>
template<typename U>
Vec<U, N> Vec<T, N>::map2(Vec<T, N> rhs, std::function<U(T, T)> f) const
{
    Vec<U, N> result;
    for (size_t i = 0; i < N; i++)
        result[i] = f(data[i], rhs[i]);
    return result;
}

/**
 * Sum two vectors.
 */
template<typename T, size_t N>
Vec<T, N> Vec<T, N>::operator+(const Vec<T, N> &v) const
{
    return this->map2<T>(v, [](T lhs, T rhs) { return lhs + rhs; });
}

/**
 * Subtract two vectors.
 */
template<typename T, size_t N>
Vec<T, N> Vec<T, N>::operator-(const Vec<T, N> &v) const
{
    return this->map2<T>(v, [](T lhs, T rhs) { return lhs - rhs; });
}

/**
 * Multiply two vectors.
 */
template<typename T, size_t N>
Vec<T, N> Vec<T, N>::operator*(const Vec<T, N> &v) const
{
    return this->map2<T>(v, [](T lhs, T rhs) { return lhs * rhs; });
}

/**
 * Divide two vectors.
 * Throws an exception if any element of the second vector is zero.
 */
template<typename T, size_t N>
Vec<T, N> Vec<T, N>::operator/(const Vec<T, N> &v) const
{
    return this->map2<T>(v, [](T lhs, T rhs) {
        if (rhs == 0)
            throw std::runtime_error("Division by zero");
        return lhs / rhs;
    });
}

/**
 * Add a scalar to a vector.
 */
template<typename T, size_t N>
Vec<T, N> Vec<T, N>::operator+(const T &s) const
{
    return this->map<T>([&s](T x) { return x + s; });
}

/**
 * Subtract a scalar from a vector.
 */
template<typename T, size_t N>
Vec<T, N> Vec<T, N>::operator-(const T &s) const
{
    return this->map<T>([&s](T x) { return x - s; });
}

/**
 * Multiply a vector by a scalar.
 */
template<typename T, size_t N>
Vec<T, N> Vec<T, N>::operator*(const T &s) const
{
    return this->map<T>([&s](T x) { return x * s; });
}

/**
 * Divide a vector by a scalar.
 * Throws an exception if the scalar is zero.
 */
template<typename T, size_t N>
Vec<T, N> Vec<T, N>::operator/(const T &s) const
{
    if (s == 0)
        throw std::runtime_error("Division by zero");
    return this->map<T>([&s](T x) { return x / s; });
}

/**
 * Negate a vector.
 * This is the same as multiplying by -1.
 */
template<typename T, size_t N>
Vec<T, N> Vec<T, N>::operator-() const
{
    return this->map<T>([](T x) { return -x; });
}

/**
 * Assign a vector to another vector.
 */
template<typename T, size_t N>
Vec<T, N> &Vec<T, N>::operator=(const Vec<T, N> &v)
{
    std::copy(v.data.begin(), v.data.end(), data.begin());
    return *this;
}

/**
 * Add-assign a vector to another vector.
 */
template<typename T, size_t N>
Vec<T, N> &Vec<T, N>::operator+=(const Vec<T, N> &v)
{
    return *this = *this + v;
}

/**
 * Subtract-assign a vector to another vector.
 */
template<typename T, size_t N>
Vec<T, N> &Vec<T, N>::operator-=(const Vec<T, N> &v)
{
    return *this = *this - v;
}

/**
 * Multiply-assign a vector to another vector.
 */
template<typename T, size_t N>
Vec<T, N> &Vec<T, N>::operator*=(const Vec<T, N> &v)
{
    return *this = *this * v;
}

/**
 * Divide-assign a vector to another vector.
 * Throws an exception if the scalar is zero.
 */
template<typename T, size_t N>
Vec<T, N> &Vec<T, N>::operator/=(const Vec<T, N> &v)
{
    if (v[0] == 0)
        throw std::runtime_error("Division by zero");
    return *this = *this / v;
}

/**
 * Add-assign a scalar to a vector
 */
template<typename T, size_t N>
Vec<T, N> &Vec<T, N>::operator+=(const T &v) {
    return *this = *this + v;
}

/**
 * Subtract-assign a scalar to a vector
 */
template<typename T, size_t N>
Vec<T, N> &Vec<T, N>::operator-=(const T &v) {
    return *this = *this - v;
}

/**
 * Multiply-assign a scalar to a vector
 */
template<typename T, size_t N>
Vec<T, N> &Vec<T, N>::operator*=(const T &v) {
    return *this = *this * v;
}

/**
 * Divide-assign a scalar to a vector
 */
template<typename T, size_t N>
Vec<T, N> &Vec<T, N>::operator/=(const T &v) {
    return *this = *this / v;
}

/**
 * Find values where vectors are equal
 */
template<typename T, size_t N>
Vec<bool, N> Vec<T, N>::operator==(const Vec<T, N> &v) const {
    return this->map2<bool>(v, [](T a, T b) { return a == b; });
}

/**
 * Find values where vectors are not equation
 */
template<typename T, size_t N>
Vec<bool, N> Vec<T, N>::operator!=(const Vec<T, N> &v) const {
    return this->map2<bool>(v, [](T a, T b) { return a != b; });
}

/**
 * Find values where vectors are less than another vector
 */
template<typename T, size_t N>
Vec<bool, N> Vec<T, N>::operator<(const Vec<T, N> &v) const {
    return this->map2<bool>(v, [](T a, T b) { return a < b; });
}

/**
 * Find values where vectors are greater than another vector
 */
template<typename T, size_t N>
Vec<bool, N> Vec<T, N>::operator>(const Vec<T, N> &v) const {
    return this->map2<bool>(v, [](T a, T b) { return a > b; });
}

/**
 * Find values where vectors are less than or equal to another vector
 */
template<typename T, size_t N>
Vec<bool, N> Vec<T, N>::operator<=(const Vec<T, N> &v) const {
    return this->map2<bool>(v, [](T a, T b) { return a <= b; });
}

/**
 * Find values where vectors are greater than or equal to another vector
 */
template<typename T, size_t N>
Vec<bool, N> Vec<T, N>::operator>=(const Vec<T, N> &v) const {
    return this->map2<bool>(v, [](T a, T b) { return a >= b; });
}

/**
 * Get reference to a vector element by index.
 */
template<typename T, size_t N>
T &Vec<T, N>::operator[](size_t i) {
    return data[i];
}

/**
 * Get const reference to a vector element by index.
 */
template<typename T, size_t N>
const T &Vec<T, N>::operator[](size_t i) const {
    return data[i];
}

/**
 * Get vector magnitude using L2 norm.
 */
template<typename T, size_t N>
T Vec<T, N>::mag() const {
    return std::sqrt(this->dot(*this));
}

/**
 * Get squared vector magnitude using L2 norm.
 */
template<typename T, size_t N>
T Vec<T, N>::mag2() const {
    return this->dot(*this);
}

/**
 * Normalize vector
 */
template<typename T, size_t N>
Vec<T, N> Vec<T, N>::normalize(float tol) const {
    T mag = this->magnitude();
    if (mag > tol)
        return *this / mag;
    else
        return Vec<T, N>();
}

/**
 * Fast normalize vector
 */
template<typename T, size_t N>
Vec<T, N> Vec<T, N>::normalizeFast() const {
    T mag = fisqrt(this->mag2());
    if (mag > 0)
        return *this / mag;
    else
        return Vec<T, N>();
}

/**
 * Dot product of two vectors.
 */
template<typename T, size_t N>
T Vec<T, N>::dot(const Vec<T, N> &v) const {
    return this->map2<T>(v, [](T a, T b) { return a * b; }).sum();
}

/**
 * Sum of all elements in a vector.
 */
template<typename T, size_t N>
T Vec<T, N>::sum() const {
    T sum = 0;
    for(auto i: data) {
        sum += i;
    }
    return sum;
}

/**
 * Product of all elements in a vector.
 */
template<typename T, size_t N>
T Vec<T, N>::prod() const {
    T prod = 0;
    for(auto i: data) {
        prod *= i;
    }
    return prod;
}

/**
 * Minimum element in a vector.
 */
template<typename T, size_t N>
T Vec<T, N>::min() const {
    return *std::min_element(data.begin(), data.end());
}

/**
 * Maximum element in a vector.
 */
template<typename T, size_t N>
T Vec<T, N>::max() const {
    return *std::max_element(data.begin(), data.end());
}

/**
 * Add scalar to all elements in a vector.
 */
template<typename T, size_t N>
Vec<T, N> operator+(const T &v, const Vec<T, N> &u) {
    return u + v;
}

/**
 * Subtract all elements in a vector from a scalar.
 */
template<typename T, size_t N>
Vec<T, N> operator-(const T &v, const Vec<T, N> &u) {
    return -u + v;
}

/**
 * Multiply all elements in a vector by a scalar.
 */
template<typename T, size_t N>
Vec<T, N> operator*(const T &v, const Vec<T, N> &u) {
    return u * v;
}

/**
 * Divide a scalar by all elements in a vector.
 * If an element of the vector is 0, throw an exception.
 */
template<typename T, size_t N>
Vec<T, N> operator/(const T &v, const Vec<T, N> &u) {
    return u.template map<T>([v](T a) {
        if (a == 0)
            throw std::runtime_error("Division by zero");
        return v / a;
    });
}

/**
 * Cross Product for 3D vectors
 */
template <typename T>
Vec<T, 3> cross(const Vec<T, 3> &v1, const Vec<T, 3> &v2) {
    return {
            v1[1] * v2[2] - v1[2] * v2[1],
            v1[2] * v2[0] - v1[0] * v2[2],
            v1[0] * v2[1] - v1[1] * v2[0]
    };
}

template <typename T, size_t N>
std::ostream &operator<<( std::ostream &os, const Vec<T, N> &v)
{
    if (N <= 0) {
        os << "[ ]";
        return os;
    }
    os << "[ ";
    for (size_t i = 0; i < N - 1; ++i) {
        os << v[i] << ", ";
    }
    os << v[N - 1] << " ]";
    return os;
}

#endif
