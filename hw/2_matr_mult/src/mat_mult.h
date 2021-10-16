#pragma once

#include <cstddef>

void mat_mul_mat_ijk(const double *A, const double *B, double *C, size_t N);
void mat_mul_mat_kij(const double *A, const double *B, double *C, size_t N);
void mat_mul_vec(const double *A, const double *b, double *c, size_t N);
