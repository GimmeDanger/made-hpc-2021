#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <time.h>

int in_circle(float x, float y)
{
    return (x - 0.5f) * (x - 0.5f) + (y - 0.5f) * (y - 0.5f) < 0.5f * 0.5f;
}

float monte_carlo_pi(size_t N)
{
    int i, tid, nthreads;
    unsigned int seed;
    float x, y;
    float start, end;
    size_t N_circle = 0;
    size_t N_total_rounds = 0;

    start = omp_get_wtime();
    #pragma omp parallel shared(N) private(i, tid, nthreads, seed, x, y) reduction(+: N_circle, N_total_rounds)
    {
        tid = omp_get_thread_num();
        nthreads = omp_get_num_threads();
        seed = tid * (unsigned int) time(NULL);

        #pragma omp parallel for reduction(+: N_circle, N_total_rounds)
        for (i = 0; i < (N / nthreads + N % nthreads); ++i)
        {
	    x = (float)(rand_r(&seed)) / RAND_MAX;
       	    y = (float)(rand_r(&seed)) / RAND_MAX;
	    if (in_circle(x, y))
	    {
	        N_circle++;
	    }
	    N_total_rounds++;
	    // printf("tid = %d i = %d\n", tid, i);
        }
    }
    end = omp_get_wtime();
    printf("N = %zu round of Monte-Carlo, elapsed time = %f ms\n",
           N, (end - start) * 1000);
    return ((float)(N_circle) / N) / (0.5f * 0.5f);
}

int main()
{
    float pi;
    const size_t N = 100000000;
    pi = monte_carlo_pi(N);
    printf("Calculated PI = %f\n", pi);
    printf("True PI = %f\n", M_PI);
    printf("Diff = %f\n", fabs(pi - M_PI));
    return 0;
}
