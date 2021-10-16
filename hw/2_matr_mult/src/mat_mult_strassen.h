#pragma once

#include <cstddef>

bool is_pow_2(size_t N);
void mat_mul_mat_strassen_naive(const double *A, const double *B, double *C, size_t N, size_t FBN);
