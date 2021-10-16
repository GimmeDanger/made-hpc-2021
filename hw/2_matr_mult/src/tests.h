#pragma once

#include "helpers.h"
#include "mat_mult_strassen.h"
#include "mat_mult.h"

#include <cstddef>
#include <cassert>

inline double test_mat_mul_vec(const double *A, const double *b, double *c, size_t N) {
  return benchmark([&]() {
    mat_mul_vec(A, b, c, N);
  });
}

inline double test_mat_mul_mat_kij(const double *A, const double *B, double *C, size_t N) {
  return benchmark([&]() {
    mat_mul_mat_kij(A, B, C, N);
  });
}

template<size_t FBN>
double test_mat_mult_mat_strassen_naive(const double *A, const double *B, double *C, size_t N) {
  assert(is_pow_2(N));
  return benchmark([&]() {
    mat_mul_mat_strassen_naive(A, B, C, N, FBN);
  });
}

template<int NRuns, typename F, typename G>
void test(F F_test, G G_unit, const std::string& test_tag) {
  printf("test %s started\n", test_tag.c_str());
  std::vector<double> runtimes(NRuns);
  double average_runtime = 0.0;
  for (int n = 0; n < NRuns; n++) {
    runtimes[n] = F_test();
    G_unit();
    printf("runtime %lf seconds\n", runtimes[n]);
    average_runtime += runtimes[n] / NRuns;
  }
  printf("average runtime %lf seconds\n", average_runtime);
  printf("---------------------------------\n");
}