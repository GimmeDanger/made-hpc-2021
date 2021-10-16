#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include <random>
#include <cassert>

#define DBG_ASSERT 0

void ZeroMatrix(double* A, size_t N) {
  for (size_t i = 0; i < N; i++) {
    for (size_t j = 0; j < N; j++) {
      A[i * N + j] = 0.0;
    }
  }
}

void CheckMatrix(const double* A, const double* B, const size_t N) {
  for (size_t i = 0; i < N; i++) {
    for (size_t j = 0; j < N; j++) {
      if (abs(A[i * N + j] - B[i * N + j]) > 1.e-8)
        assert(false);
    }
  }
}

void RandomMatrix(double * A, size_t N) {
  std::random_device rd;  // Will be used to obtain a seed for the random number engine
  std::mt19937 gen(rd());  // Standard mersenne_twister_engine seeded with rd()
  std::uniform_real_distribution<> dis(1.0, 100);
  for (size_t i = 0; i < N; i++) {
    for (size_t j = 0; j < N; j++) {
      A[i * N + j] = dis(gen);
    }
  }
}

template<typename Func>
double benchmark(Func func_to_check) {
  const auto start = std::chrono::steady_clock::now();
  func_to_check();
  const auto end = std::chrono::steady_clock::now();
  std::chrono::duration<double> elapsed = end - start;
  return elapsed.count();
}

void CalcMatMulTime_ijk_impl(const double *A, const double *B, double *C, size_t N) {
  for (size_t i = 0; i < N; i++)
    for (size_t j = 0; j < N; j++) {
      for (size_t k = 0; k < N; k++)
        C[i * N + j] = C[i * N + j] + A[i * N + k] * B[k * N + j];
    }
}

double CalcMatMulTime_ijk(const double *A, const double *B, double *C, size_t N) {
  ZeroMatrix(&C[0], N);
  return benchmark([&]() {
    CalcMatMulTime_ijk_impl(A, B, C, N);
  });
}

void CalcMatMulTime_jik_impl(const double * A, const double *B, double *C, size_t N) {
  for (size_t j = 0; j < N; j++)
    for (size_t i = 0; i < N; i++) {
      for (size_t k = 0; k < N; k++)
        C[i * N + j] = C[i * N + j] + A[i * N + k] * B[k * N + j];
    }
}

double CalcMatMulTime_jik(const double * A, const double *B, double *C, size_t N) {
  ZeroMatrix(&C[0], N);
  return benchmark([&]() {
    CalcMatMulTime_jik_impl(A, B, C, N);
  });
}

void CalcMatMulTime_kij_impl(const double *A, const double *B, double *C, size_t N) {
  for (size_t k = 0; k < N; k++)
    for (size_t i = 0; i < N; i++) {
      for (size_t j = 0; j < N; j++)
        C[i * N + j] = C[i * N + j] + A[i * N + k] * B[k * N + j];
    }
}

double CalcMatMulTime_kij(const double *A, const double *B, double *C, size_t N) {
  ZeroMatrix(&C[0], N);
  return benchmark([&]() {
    CalcMatMulTime_kij_impl(A, B, C, N);
  });
}

void CalcMatMulTime_kij_opt_impl(const double *A, const double *B, double *C, size_t N) {
  for (size_t k = 0; k < N; k++) {
    for (size_t i = 0; i < N; i++) {
      size_t dummy = i * N;
      for (size_t j = 0; j < N; j++)
        C[dummy + j] = C[dummy + j] + A[dummy + k] * B[k * N + j];
    }
  }
}

double CalcMatMulTime_kij_opt(const double *A, const double *B, double *C, size_t N) {
  ZeroMatrix(&C[0], N);
  return benchmark([&]() {
    CalcMatMulTime_kij_opt_impl(A, B, C, N);
  });
}

void CalcMatMulTime_kij_my_opt_impl(const double *A, const double *B, double *C, size_t N) {
  for (size_t k = 0; k < N; k++) {
    auto* pB = B + k * N;
    for (size_t i = 0; i < N; i++) {
      auto* pC = C + i * N;
      auto a_ik = A[i * N + k];
      for (size_t j = 0; j < N; j++)
        pC[j] += a_ik * pB[j];
    }
  }
}

double CalcMatMulTime_kij_my_opt(const double *A, const double *B, double *C, size_t N) {
  ZeroMatrix(&C[0], N);
  return benchmark([&]() {
    CalcMatMulTime_kij_my_opt_impl(A, B, C, N);
  });
}

int main() {
  const size_t N = 1024;
  const int NRuns = 20;

  double *runtimes;
  double *A, *B, *C, *D;

  A = (double *)malloc(N * N * sizeof(double));
  B = (double *)malloc(N * N * sizeof(double));
  C = (double *)malloc(N * N * sizeof(double));
  D = (double *)malloc(N * N * sizeof(double));
  runtimes = (double *)malloc(NRuns * sizeof(double));
  double average_runtime = 0.0;

  RandomMatrix(&A[0], N);
  RandomMatrix(&B[0], N);

//   // ijk ordering
//   average_runtime = 0.0;
//   for (int n = 0; n < 0; n++) {
//     runtimes[n] = CalcMatMulTime_ijk(&A[0], &B[0], &C[0], N);
//     if (DBG_ASSERT) {
//       CalcMatMulTime_kij_opt(&A[0], &B[0], &D[0], N);
//       CheckMatrix(&C[0], &D[0], N);
//     }
//     printf("runtime %lf seconds\n", runtimes[n]);
//     average_runtime += runtimes[n] / NRuns;
//   }
//
//   printf("average runtime ijk %lf seconds\n", average_runtime);
//   printf("---------------------------------\n");

//  // jik ordering
//  average_runtime = 0.0;
//  for (int n = 0; n < NRuns; n++) {
//    runtimes[n] = CalcMatMulTime_jik(&A[0], &B[0], &C[0], N);
//    if (DBG_ASSERT) {
//      CalcMatMulTime_kij_opt(&A[0], &B[0], &D[0], N);
//      CheckMatrix(&C[0], &D[0], N);
//    }
//    printf("runtime %lf seconds\n", runtimes[n]);
//    average_runtime += runtimes[n] / NRuns;
//  }
//
//  printf("average runtime jik %lf seconds\n", average_runtime);
//  printf("---------------------------------\n");

  // // kij ordering
  // average_runtime = 0.0;
  // for (int n = 0; n < NRuns; n++) {
  //   runtimes[n] = CalcMatMulTime_kij(&A[0], &B[0], &C[0], N);
  //   if (DBG_ASSERT) {
  //     CalcMatMulTime_kij_opt(&A[0], &B[0], &D[0], N);
  //     CheckMatrix(&C[0], &D[0], N);
  //   }
  //   printf("runtime %lf seconds\n", runtimes[n]);
  //   average_runtime += runtimes[n] / NRuns;
  // }
  // printf("average runtime kij %lf seconds\n", average_runtime);
  // printf("---------------------------------\n");

  // kij ordering naive optimization (useless for -O3)
  average_runtime = 0.0;
  for (int n = 0; n < NRuns; n++) {
    runtimes[n] = CalcMatMulTime_kij_opt(&A[0], &B[0], &C[0], N);
    if (DBG_ASSERT) {
      CalcMatMulTime_ijk(&A[0], &B[0], &D[0], N);
      CheckMatrix(&C[0], &D[0], N);
    }
    printf("runtime %lf seconds\n", runtimes[n]);
    average_runtime += runtimes[n] / NRuns;
  }
  printf("average runtime kij naive opt %lf seconds\n", average_runtime);
  printf("---------------------------------\n");

  // kij ordering more naive optimizations
  average_runtime = 0.0;
  for (int n = 0; n < NRuns; n++) {
    runtimes[n] = CalcMatMulTime_kij_my_opt(&A[0], &B[0], &C[0], N);
    if (DBG_ASSERT) {
      CalcMatMulTime_kij_opt(&A[0], &B[0], &D[0], N);
      CheckMatrix(&C[0], &D[0], N);
    }
    printf("runtime %lf seconds\n", runtimes[n]);
    average_runtime += runtimes[n] / NRuns;
  }
  printf("average runtime kij more naive opt %lf seconds\n", average_runtime);
  printf("---------------------------------\n");

  free(A);
  free(B);
  free(C);
  free(D);
  return 0;
}
