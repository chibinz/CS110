#include <omp.h>
#include <stdio.h>

int main()
{
#pragma omp parallel
    {
        int thread_ID = omp_get_thread_num();
        printf(" hello world %d\n", thread_ID);
    }
    return 0;
}
