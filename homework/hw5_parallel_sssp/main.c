#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Your shortest distance algorithm. */
extern void dist(int max_threads);

/* Print out the usage message. */
static void usage(char const *name)
{
    fprintf(stderr, "Usage: %s N\n", name);
    fprintf(stderr, "Where N is the maximum number of threads the host machine supports.\n");
}

int main(int argc, char const *argv[])
{
    /* Check the number of parameters */
    if (argc != 2)
    {
        usage(argv[0]);
        exit(1);
    }

    /* Get the maximum number of threads */
    int max_threads = atoi(argv[1]);

    /* Measure the running time. */
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    dist(max_threads);
    clock_gettime(CLOCK_MONOTONIC, &end);
    double duration = (double)(end.tv_sec - start.tv_sec) + (double)(end.tv_nsec - start.tv_nsec) / (double)1e9;
    fprintf(stderr, "Time cost: %f seconds\n", duration);

    return 0;
}
