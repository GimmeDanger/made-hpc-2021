#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include <random>

void ZeroMatrix(double* A, size_t N) {
  for (size_t i = 0; i < N; i++) {
    for (size_t j = 0; j < N; j++) {
      A[i * N + j] = 0.0;
    }
  }
}

void RandomMatrix(double * A, size_t N) {
  std::random_device rd;  // Will be used to obtain a seed for the random number engine
  std::mt19937 gen(rd());  // Standard mersenne_twister_engine seeded with rd()
  std::uniform_real_distribution<> dis(0.0, std::numeric_limits<int>::max());
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

double CalcMatMulTime_ijk(const double *A, const double *B, double *C, size_t N) {
  ZeroMatrix(&C[0], N);
  return benchmark([&]() {
    for (size_t i = 0; i < N; i++)
      for (size_t j = 0; j < N; j++) {
        for (size_t k = 0; k < N; k++)
          C[i * N + j] = C[i * N + j] + A[i * N + k] * B[k * N + j];
      }
  });
}

double CalcMatMulTime_jik(const double * A, const double *B, double *C, size_t N) {
  ZeroMatrix(&C[0], N);
  return benchmark([&]() {
    for (size_t j = 0; j < N; j++)
      for (size_t i = 0; i < N; i++) {
        for (size_t k = 0; k < N; k++)
          C[i * N + j] = C[i * N + j] + A[i * N + k] * B[k * N + j];
      }
  });
}

double CalcMatMulTime_kij(const double *A, const double *B, double *C, size_t N) {
  ZeroMatrix(&C[0], N);
  return benchmark([&]() {
    for (size_t k = 0; k < N; k++)
      for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < N; j++)
          C[i * N + j] = C[i * N + j] + A[i * N + k] * B[k * N + j];
      }
  });
}

double CalcMatMulTime_kij_opt(const double *A, const double *B, double *C, size_t N) {
  ZeroMatrix(&C[0], N);
  return benchmark([&]() {
    for (size_t k = 0; k < N; k++)
      for (size_t i = 0; i < N; i++) {
        size_t dummy = i * N;
        for (size_t j = 0; j < N; j++)
          C[dummy + j] = C[dummy + j] + A[dummy + k] * B[k * N + j];
      }
  });
}

int main()
{
    const size_t N = 512;
    const int NRuns = 5;

    double *runtimes;
    double *A, *B, *C;

    A = (double *) malloc(N * N * sizeof(double));
    B = (double *) malloc(N * N * sizeof(double));
    C = (double *) malloc(N * N * sizeof(double));
    runtimes = (double *) malloc(NRuns * sizeof(double));

    RandomMatrix(&A[0], N);
    RandomMatrix(&B[0], N);

    // ijk ordering
    double average_runtime = 0.0;
    for(int n=0; n<NRuns; n++)
    {
        runtimes[n]=CalcMatMulTime_ijk(&A[0], &B[0], &C[0], N);
        printf("runtime %lf seconds\n", runtimes[n]);
        average_runtime += runtimes[n]/NRuns;
    }

    printf("average runtime ijk %lf seconds\n", average_runtime);
    printf("---------------------------------\n");


// jik ordering
    average_runtime = 0.0;
    for(int n=0; n<NRuns; n++)
    {
        runtimes[n]=CalcMatMulTime_jik(&A[0], &B[0], &C[0], N);
        printf("runtime %lf seconds\n", runtimes[n]);
        average_runtime += runtimes[n]/NRuns;
    }

    printf("average runtime jik %lf seconds\n", average_runtime);
    printf("---------------------------------\n");


    // kij ordering
    average_runtime = 0.0;
    for(int n=0; n<NRuns; n++)
    {
        runtimes[n]=CalcMatMulTime_kij(&A[0], &B[0], &C[0], N);
        printf("runtime %lf seconds\n", runtimes[n]);
        average_runtime += runtimes[n]/NRuns;
    }
    printf("average runtime kij %lf seconds\n", average_runtime);
    printf("---------------------------------\n");

    // kij ordering naive optimization (useless for -O3)
    average_runtime = 0.0;
    for(int n=0; n<NRuns; n++)
    {
        runtimes[n]=CalcMatMulTime_kij_opt(&A[0], &B[0], &C[0], N);
        printf("runtime %lf seconds\n", runtimes[n]);
        average_runtime += runtimes[n]/NRuns;
    }
    printf("average runtime kij opt %lf seconds\n", average_runtime);
    printf("---------------------------------\n");

    free(A);
    free(B);
    free(C);
    return 0;
}