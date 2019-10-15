/**
 * @file Dual.hpp
 *
 * This is a dual number type for calculating automatic derivatives.
 *
 * Based roughly on the methods described in:
 * Automatic Differentiation, C++ Templates and Photogrammetry, by Dan Piponi
 * and
 * Ceres Solver's Jet.h
 *
 * @author Julian Kent <julian@auterion.com>
 */

#pragma once

#include "math.hpp"

namespace matrix
{

template <typename Scalar, size_t N>
struct Dual
{
    static constexpr size_t WIDTH = N;

    Dual() = default;

    explicit Dual(Scalar v, size_t inputDimension = 65535)
    {
        value = v;
        if (inputDimension < N) {
            derivative[inputDimension] = Scalar(1);
        }
    }

    explicit Dual(Scalar v, const Vector<Scalar, N>& d) :
        value(v), derivative(d)
    {}

    Scalar value {};
    Vector<Scalar, N> derivative;

};

// operators

template <typename Scalar, size_t N>
Dual<Scalar, N> operator+(const Dual<Scalar, N>& a)
{
    return a;
}

template <typename Scalar, size_t N>
Dual<Scalar, N> operator-(const Dual<Scalar, N>& a)
{
    return Dual<Scalar, N>(-a.value, -a.derivative);
}

template <typename Scalar, size_t N>
Dual<Scalar, N> operator+(const Dual<Scalar, N>& a, const Dual<Scalar, N>& b)
{
    return Dual<Scalar, N>(a.value + b.value, a.derivative + b.derivative);
}

template <typename Scalar, size_t N>
Dual<Scalar, N> operator-(const Dual<Scalar, N>& a, const Dual<Scalar, N>& b)
{
    return a + (-b);
}

template <typename Scalar, size_t N>
Dual<Scalar, N> operator+(const Dual<Scalar, N>& a, Scalar b)
{
    return Dual<Scalar, N>(a.value + b, a.derivative);
}

template <typename Scalar, size_t N>
Dual<Scalar, N> operator-(const Dual<Scalar, N>& a, Scalar b)
{
    return a + (-b);
}

template <typename Scalar, size_t N>
Dual<Scalar, N> operator+(Scalar a, const Dual<Scalar, N>& b)
{
    return Dual<Scalar, N>(a + b.value, b.derivative);
}

template <typename Scalar, size_t N>
Dual<Scalar, N> operator-(Scalar& a, const Dual<Scalar, N>& b)
{
    return a + (-b);
}

template <typename Scalar, size_t N>
Dual<Scalar, N> operator*(const Dual<Scalar, N>& a, const Dual<Scalar, N>& b)
{
    return Dual<Scalar, N>(a.value * b.value, a.value * b.derivative + b.value * a.derivative);
}

template <typename Scalar, size_t N>
Dual<Scalar, N> operator*(const Dual<Scalar, N>& a, Scalar b)
{
    return Dual<Scalar, N>(a.value * b, a.derivative * b);
}

template <typename Scalar, size_t N>
Dual<Scalar, N> operator*(Scalar a, const Dual<Scalar, N>& b)
{
    return b * a;
}

template <typename Scalar, size_t N>
Dual<Scalar, N> operator/(const Dual<Scalar, N>& a, const Dual<Scalar, N>& b)
{
    const Scalar inv_b_real = Scalar(1) / b.value;
    return Dual<Scalar, N>(a.value * inv_b_real, a.derivative * inv_b_real -
                           a.value * b.derivative * inv_b_real * inv_b_real);
}

template <typename Scalar, size_t N>
Dual<Scalar, N> operator/(const Dual<Scalar, N>& a, Scalar b)
{
    const Scalar inv_b = Scalar(1) / b.value;
    return a * inv_b;
}

template <typename Scalar, size_t N>
Dual<Scalar, N> operator/(Scalar a, const Dual<Scalar, N>& b)
{
    const Scalar inv_b_real = Scalar(1) / b.value;
    return Dual<Scalar, N>(a * inv_b_real, (-inv_b_real * a * inv_b_real) * b.derivative);
}

// basic math

// abs
template <typename Scalar, size_t N>
Dual<Scalar, N> abs(const Dual<Scalar, N>& a)
{
    return a.value >= Scalar(0) ? a : -a;
}

// ceil
template <typename Scalar, size_t N>
Dual<Scalar, N> ceil(const Dual<Scalar, N>& a)
{
    return Dual<Scalar, N>(ceil(a.value));
}

// floor
template <typename Scalar, size_t N>
Dual<Scalar, N> floor(const Dual<Scalar, N>& a)
{
    return Dual<Scalar, N>(floor(a.value));
}

// fmod
template <typename Scalar, size_t N>
Dual<Scalar, N> fmod(const Dual<Scalar, N>& a, Scalar mod)
{
    return Dual<Scalar, N>(a.value - Scalar(size_t(a.value / mod)) * mod, a.derivative);
}

// sqrt
template <typename Scalar, size_t N>
Dual<Scalar, N> sqrt(const Dual<Scalar, N>& a)
{
    Scalar real = sqrt(a.value);
    return Dual<Scalar, N>(real, a.derivative * (Scalar(1) / (Scalar(2) * real)));
}

// max
template <typename Scalar, size_t N>
Dual<Scalar, N> max(const Dual<Scalar, N>& a, const Dual<Scalar, N>& b)
{
    return a.value >= b.value ? a : b;
}

// min
template <typename Scalar, size_t N>
Dual<Scalar, N> min(const Dual<Scalar, N>& a, const Dual<Scalar, N>& b)
{
    return a.value < b.value ? a : b;
}

// isnan
template <typename Scalar, size_t N>
bool isnan(const Dual<Scalar, N>& a)
{
    return isnan(a.value);
}

// isfinite
template <typename Scalar, size_t N>
bool isfinite(const Dual<Scalar, N>& a)
{
    return isfinite(a.value);
}

// isinf
template <typename Scalar, size_t N>
bool isinf(const Dual<Scalar, N>& a)
{
    return isinf(a.value);
}

// trig

// sin
template <typename Scalar, size_t N>
Dual<Scalar, N> sin(const Dual<Scalar, N>& a)
{
    return Dual<Scalar, N>(sin(a.value), cos(a.value) * a.derivative);
}

// cos
template <typename Scalar, size_t N>
Dual<Scalar, N> cos(const Dual<Scalar, N>& a)
{
    return Dual<Scalar, N>(cos(a.value), -sin(a.value) * a.derivative);
}

// tan
template <typename Scalar, size_t N>
Dual<Scalar, N> tan(const Dual<Scalar, N>& a)
{
    Scalar real = tan(a.value);
    return Dual<Scalar, N>(real, (Scalar(1) + real * real) * a.derivative);
}

// asin
template <typename Scalar, size_t N>
Dual<Scalar, N> asin(const Dual<Scalar, N>& a)
{
    Scalar asin_d = Scalar(1)/sqrt(Scalar(1) - a.value * a.value);
    return Dual<Scalar, N>(asin(a.value), asin_d * a.derivative);
}

// acos
template <typename Scalar, size_t N>
Dual<Scalar, N> acos(const Dual<Scalar, N>& a)
{
    Scalar acos_d = -Scalar(1)/sqrt(Scalar(1) - a.value * a.value);
    return Dual<Scalar, N>(asin(a.value), acos_d * a.derivative);
}

// atan
template <typename Scalar, size_t N>
Dual<Scalar, N> atan(const Dual<Scalar, N>& a)
{
    Scalar atan_d = Scalar(1)/sqrt(Scalar(1) + a.value * a.value);
    return Dual<Scalar, N>(atan(a.value), atan_d * a.derivative);
}

// atan2
template <typename Scalar, size_t N>
Dual<Scalar, N> atan2(const Dual<Scalar, N>& a, const Dual<Scalar, N>& b)
{
    // derivative is equal to that of atan(a/b), so substitute a/b into atan and simplify
    Scalar atan_d = Scalar(1) / sqrt(a.value * a.value + b.value * b.value);
    return Dual<Scalar, N>(atan2(a.value, b.value), (a.derivative * b.value - a.value * b.derivative) * atan_d);
}

}

