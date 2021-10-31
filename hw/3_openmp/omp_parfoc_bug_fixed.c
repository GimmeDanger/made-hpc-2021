#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

int main (int argc, char *argv[])
{
    const size_t N = 100;
    const size_t chunk = 4;

    int i, tid;
    float a[N], b[N], c[N], d[N];

    for (i = 0; i < N; ++i)
    {
        a[i] = b[i] = (float)i;
	d[i] = a[i] + b[i];
    }

    #pragma omp parallel for \
    	shared(a,b,c) \
	private(i,tid) \
        schedule(static, chunk)
    for (i = 0; i < N; ++i)
    {
        c[i] = a[i] + b[i];
	tid = omp_get_thread_num();
	printf("tid = %d, c[%d] = %f\n", tid, i, c[i]);
    }

    // check results
    for (i = 0; i < N; ++i)
    {
	if (fabs(c[i] - d[i]) > 1.e-6)
	{
	    printf("assert: i=%d, %f != %f", i, c[i], d[i]);
            assert(0);
	}
    }

    return 0;
}
