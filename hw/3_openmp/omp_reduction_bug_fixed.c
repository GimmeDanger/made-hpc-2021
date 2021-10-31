#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

float dotprod(float * a, float * b, size_t N)
{
    int i, tid;
    float sum = 0;

#pragma omp parallel for \
    shared(a, b, N) \
    reduction(+: sum)
    for (i = 0; i < N; ++i)
    {
        sum += a[i] * b[i];
        tid = omp_get_thread_num();
        printf("tid = %d i = %d\n", tid, i);
    }

    return sum;
}

int main (int argc, char *argv[])
{
    const size_t N = 100;
    int i, tid;
    float sum, seq_sum;
    float a[N], b[N];

    seq_sum = 0;
    for (i = 0; i < N; ++i)
    {
        a[i] = b[i] = (float)i;
	seq_sum += a[i] * b[i];
    }

    sum = dotprod(a, b, N);
    assert(fabs(sum - seq_sum) < 1.e-7);
    printf("Sum = %f, seq_sum = %f\n", sum, seq_sum);

    return 0;
}
