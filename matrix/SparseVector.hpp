/**
 * @file SparseVector.hpp
 *
 * SparseVector class.
 *
 * @author Kamil Ritz <kritz@ethz.ch>
 * @author Julian Kent <julian@auterion.com>
 *
 */

#pragma once

#include "math.hpp"

namespace matrix {
template<int N> struct force_constexpr_eval {
    static const int value = N;
};

// Vector that only store nonzero elements,
// which indices are specified as parameter pack
template<typename Type, size_t M, int... Idxs>
class SparseVector {
private:
    static constexpr size_t N = sizeof...(Idxs);
    static constexpr int _indices[N] {Idxs...};

    static constexpr bool duplicateIndices() {
        for (int i = 0; i < N; i++)
            for (int j = 0; j < i; j++)
                if (_indices[i] == _indices[j])
                    return true;
        return false;
    }
    static constexpr int findMaxIndex() {
        int maxIndex = -1;
        for (int i = 0; i < N; i++) {
            if (maxIndex < _indices[i]) {
                maxIndex = _indices[i];
            }
        }
        return maxIndex;
    }
    static_assert(duplicateIndices() == false, "Duplicate indices");
    static_assert(N < M, "More entries than elements, use a dense vector");
    static_assert(findMaxIndex() < M, "Largest entry doesn't fit in sparse vector");

    Type _data[N] {};

    static constexpr int findCompressedIndex(int index) {
        int compressedIndex = -1;
        for (int i = 0; i < N; i++) {
            if (index == _indices[i]) {
                compressedIndex = i;
            }
        }
        return compressedIndex;
    }

public:
    static constexpr int non_zeros() {
        return N;
    }

    constexpr int index(int i) const {
        return SparseVector::_indices[i];
    }

    SparseVector() = default;
    SparseVector(const matrix::Vector<Type, N>& data) {
        data.copyTo(_data);
    }

    template <size_t otherVectorSize>
    SparseVector& fromDenseVector(const matrix::Vector<Type, otherVectorSize>& data) {
        static_assert(otherVectorSize > findMaxIndex(), "SparseVector index out of bounds");
        for (int i = 0; i < N; i++) {
            _data[i] = data(_indices[i]);
        }
    }

    template <int i>
    inline Type at() const {
        static constexpr int compressed_index = force_constexpr_eval<findCompressedIndex(i)>::value;
        static_assert(compressed_index >= 0, "cannot access unpopulated indices");
        return _data[compressed_index];
    }

    template <int i>
    inline Type& at() {
        static constexpr int compressed_index = force_constexpr_eval<findCompressedIndex(i)>::value;
        static_assert(compressed_index >= 0, "cannot access unpopulated indices");
        return _data[compressed_index];
    }

    void setZero() {
        for (int i = 0; i < N; i++) {
            _data[i] = Type(0);
        }
    }

    Type dot(const matrix::Vector<Type, M>& other) {
        Type accum (0);
        for (int i = 0; i < N; i++) {
            accum += _data[i] * other(_indices[i]);
        }
        return accum;
    }

    matrix::Vector<Type, M> operator+(const matrix::Vector<Type, M>& other) {
        matrix::Vector<Type, M> vec = other;
        for (int i = 0; i < N; i++) {
            vec(_indices[i]) +=  _data[i];
        }
        return vec;
    }

    SparseVector& operator+=(Type t) {
        for (int i = 0; i < N; i++) {
            _data[i] += t;
        }
        return *this;
    }
};

template<typename Type, size_t Q, size_t M, int ... Idxs>
matrix::Vector<Type, Q> operator*(const matrix::Matrix<Type, Q, M>& mat, const matrix::SparseVector<Type, M, Idxs...>& vec) {
    matrix::Vector<Type, Q> res;
    for (size_t i = 0; i < Q; i++) {
        Vector<Type, M> row = mat.row(i);
        res(i) = vec.dot(row);
    }
    return res;
}

template<int M, int ... Idxs>
template<size_t M, int ... Idxs>
using SparseVectorf = SparseVector<float, M, Idxs...>;

}
