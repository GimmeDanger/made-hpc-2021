#include "helpers.h"

#include <cmath>
#include <cassert>
#include <random>

using namespace std;

void zero_arr(double* A, size_t N) {
  for (size_t i = 0; i < N; i++) {
    A[i] = 0.0;
  }
}

void check_mat(const double* A, const double* B, const size_t N) {
  for (size_t i = 0; i < N; i++) {
    for (size_t j = 0; j < N; j++) {
      if (abs(A[i * N + j] - B[i * N + j]) > 1.e-8)
        assert(false);
    }
  }
}

void rand_arr(double *A, size_t N) {
  std::random_device rd;  // Will be used to obtain a seed for the random number engine
  std::mt19937 gen(rd());  // Standard mersenne_twister_engine seeded with rd()
  std::uniform_real_distribution<> dis(0.5, 1.0);
  for (size_t i = 0; i < N; i++) {
    A[i] = dis(gen);
  }
}