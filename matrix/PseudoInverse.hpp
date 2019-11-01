/**
 * @file PseudoInverse.hpp
 *
 * Implementation of matrix pseudo inverse
 *
 * @author Julien Lecoeur <julien.lecoeur@gmail.com>
 */

#pragma once

#include "math.hpp"

namespace matrix
{

/**
 * Full rank Cholesky factorization of A
 */
template<typename Type, size_t N>
SquareMatrix<Type, N> full_rank_cholesky(const SquareMatrix<Type, N> & A, size_t& rank);

/**
 * Geninv implementation detail
 */
template<typename Type, size_t M, size_t N, size_t R> class Geninv_impl;

/**
 * Geninv
 * Fast pseudoinverse based on full rank cholesky factorisation
 *
 * Courrieu, P. (2008). Fast Computation of Moore-Penrose Inverse Matrices, 8(2), 25–29. http://arxiv.org/abs/0804.4809
 */
template<typename Type, size_t M, size_t N>
Matrix<Type, N, M> geninv(const Matrix<Type, M, N> & G)
{
    size_t rank;
    if (M <= N) {
        SquareMatrix<Type, M> A = G * G.transpose();
        SquareMatrix<Type, M> L = full_rank_cholesky(A, rank);

        return Geninv_impl<Type, M, N, M>::geninv_M(G, L, rank);

    } else {
        SquareMatrix<Type, N> A = G.transpose() * G;
        SquareMatrix<Type, N> L = full_rank_cholesky(A, rank);

        return Geninv_impl<Type, M, N, N>::geninv_N(G, L, rank);
    }
}


#include "PseudoInverse.hxx"

} // namespace matrix

/* vim: set et fenc=utf-8 ff=unix sts=0 sw=4 ts=4 : */
