#include "tests.h"
#include "helpers.h"
#include "mat_mult.h"

#define DBG_ASSERT 0

template<int Nruns, typename Func>
void mat_mult_mat_tests(Func tested_func, const std::string& label, const std::vector<size_t>& test_N) {
  for (const size_t N : test_N) {
    std::vector<double> A(N * N), B(N * N), C(N * N), D(N * N);
    rand_arr(A.data(), N * N); rand_arr(B.data(), N * N);
    std::string local_label = label + " for N=" + std::to_string(N);
    test<Nruns>(
        // tested function
        [tested_func, &A, &B, &C, N]() {
          return tested_func(A.data(), B.data(), C.data(), N);
        },
        // unit test
        [&A, &B, &C, &D, N]() {
          if (DBG_ASSERT) {
            mat_mul_mat_ijk(A.data(), B.data(), D.data(), N);
            check_mat(C.data(), D.data(), N);
          }
        },
        // label
        local_label
    );
  }
}

template<int Nruns, typename Func>
void mat_mult_vec_tests(Func tested_func, const std::string& label, const std::vector<size_t>& test_N) {
  for (const size_t N : test_N) {
    std::vector<double> A(N * N), b(N), c(N);
    rand_arr(A.data(), N * N); rand_arr(b.data(), N);
    std::string local_label = label + " for N=" + std::to_string(N);
    test<Nruns>(
        // tested function
        [tested_func, &A, &b, &c, N]() {
          return tested_func(A.data(), b.data(), c.data(), N);
        },
        // unit test
        []() {},
        // label
        local_label
    );
  }
}

int main() {
  mat_mult_vec_tests<5/*NRuns*/>(test_mat_mul_vec,
                                 "mat mult vec test",
                                 {2000, 2048, 4000, 4096, 8000, 8182});

  mat_mult_mat_tests<5/*NRuns*/>(test_mat_mul_mat_kij,
                                 "mat mult mat kij test",
                                 {500, 512, 1000, 1024, 2000, 2048});

  mat_mult_mat_tests<5/*NRuns*/>(test_mat_mult_mat_strassen_naive<128/*FBN*/>,
                                 "mat mult mat strassen naive (fallback_N=128), ",
                                 {512, 1024, 2048, 4096});

  return 0;
}
