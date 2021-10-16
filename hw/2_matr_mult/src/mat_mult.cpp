#include "mat_mult.h"
#include "helpers.h"

// Used for unit test only
void mat_mul_mat_ijk(const double *A, const double *B, double *C, size_t N) {
  for (size_t i = 0; i < N; i++)
    for (size_t j = 0; j < N; j++) {
      for (size_t k = 0; k < N; k++) {
        if (k == 0) C[i * N + j] = 0;
        C[i * N + j] += A[i * N + k] * B[k * N + j];
      }
    }
}

void mat_mul_mat_kij(const double *A, const double *B, double *C, size_t N) {
  for (size_t k = 0; k < N; k++) {
    auto* pB = B + k * N;
    for (size_t i = 0; i < N; i++) {
      auto* pC = C + i * N;
      auto a_ik = A[i * N + k];
      for (size_t j = 0; j < N; j++) {
        if (k == 0) pC[j] = 0;
        pC[j] += a_ik * pB[j];
      }
    }
  }
}

void mat_mul_vec(const double *A, const double *b, double *c, size_t N) {
  for (size_t i = 0; i < N; i++) {
    const double* pA = A + i * N;
    for (size_t j = 0; j < N; j++) {
      if (j == 0) c[i] = 0;
      c[i] = pA[j] * b[j];
    }
  }
}
