#pragma once

#include <cstddef>
#include <cstdio>
#include <chrono>
#include <vector>
#include <string>

void zero_arr(double* A, size_t N);

void check_mat(const double* A, const double* B, const size_t N);

void rand_arr(double *A, size_t N);

template<typename Func>
double benchmark(Func func_to_check) {
  const auto start = std::chrono::steady_clock::now();
  func_to_check();
  const auto end = std::chrono::steady_clock::now();
  std::chrono::duration<double> elapsed = end - start;
  return elapsed.count();
}
