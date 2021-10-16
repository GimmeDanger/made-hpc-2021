#include "mat_mult_strassen.h"
#include "mat_mult.h"

#include <vector>

bool is_pow_2(size_t N) {
  while (N != 1) {
    if (N % 2 != 0)
      return false;
    N /= 2;
  }
  return true;
}

void mat_mul_mat_strassen_naive(const double *A, const double *B, double *C, size_t N, size_t FBN) {
  if (N <= FBN) {
    mat_mul_mat_kij(A, B, C, N);
    return;
  }
  // Init Aij, Bij
  size_t K = N / 2;
  std::vector<double> A11(K * K);
  std::vector<double> A12(K * K);
  std::vector<double> A21(K * K);
  std::vector<double> A22(K * K);
  std::vector<double> B11(K * K);
  std::vector<double> B12(K * K);
  std::vector<double> B21(K * K);
  std::vector<double> B22(K * K);
  for (size_t i = 0; i < K; i++) {
    for (size_t j = 0; j < K; j++) {
      A11[i * K + j] = A[i * N + j];
      A12[i * K + j] = A[i * N + (j + K)];
      A21[i * K + j] = A[(i + K) * N + j];
      A22[i * K + j] = A[(i + K) * N + (j + K)];
      B11[i * K + j] = B[i * N + j];
      B12[i * K + j] = B[i * N + (j + K)];
      B21[i * K + j] = B[(i + K) * N + j];
      B22[i * K + j] = B[(i + K) * N + (j + K)];
    }
  }
  // calc P components
  auto add = [K](const std::vector<double>& A, const std::vector<double>& B) {
    auto C = A;
    for (size_t i = 0; i < K * K; i++)
      C[i] += B[i];
    return C;
  };
  auto sub = [K](const std::vector<double>& A, const std::vector<double>& B) {
    auto C = A;
    for (size_t i = 0; i < K * K; i++)
      C[i] -= B[i];
    return C;
  };
  auto A11_add_A22 = add(A11, A22);
  auto A11_add_A12 = add(A11, A12);
  auto A21_add_A22 = add(A21, A22);
  auto B11_add_B22 = add(B11, B22);
  auto B11_add_B12 = add(B11, B12);
  auto B21_add_B22 = add(B21, B22);
  auto A21_sub_A11 = sub(A21, A11);
  auto A12_sub_A22 = sub(A12, A22);
  auto B12_sub_B22 = sub(B12, B22);
  auto B21_sub_B11 = sub(B21, B11);

  // Rucursive call
  std::vector<double> P1(K * K);
  std::vector<double> P2(K * K);
  std::vector<double> P3(K * K);
  std::vector<double> P4(K * K);
  std::vector<double> P5(K * K);
  std::vector<double> P6(K * K);
  std::vector<double> P7(K * K);
  mat_mul_mat_strassen_naive(A11_add_A22.data(), B11_add_B22.data(), P1.data(), K, FBN);
  mat_mul_mat_strassen_naive(A21_add_A22.data(), B11.data(), P2.data(), K, FBN);
  mat_mul_mat_strassen_naive(A11.data(), B12_sub_B22.data(), P3.data(), K, FBN);
  mat_mul_mat_strassen_naive(A22.data(), B21_sub_B11.data(), P4.data(), K, FBN);
  mat_mul_mat_strassen_naive(A11_add_A12.data(), B22.data(), P5.data(), K, FBN);
  mat_mul_mat_strassen_naive(A21_sub_A11.data(), B11_add_B12.data(), P6.data(), K, FBN);
  mat_mul_mat_strassen_naive(A12_sub_A22.data(), B21_add_B22.data(), P7.data(), K, FBN);

  // Calc C11
  for (size_t i = 0; i < K; i++) {
    for (size_t j = 0; j < K; j++) {
      C[i * N + j] = P1[i * K + j] + P4[i * K + j] - P5[i * K + j] + P7[i * K + j];
      C[i * N + (j + K)] = P3[i * K + j] + P5[i * K + j];
      C[(i + K) * N + j] = P2[i * K + j] + P4[i * K + j];
      C[(i + K) * N + (j + K)] = P1[i * K + j] - P2[i * K + j] + P3[i * K + j] + P6[i * K + j];
    }
  }
}
