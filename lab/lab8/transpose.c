#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

/* The naive transpose function as a reference. */
void transpose_naive(int n, int blocksize, int *dst, int *src) {
    for (int x = 0; x < n; x++) {
        for (int y = 0; y < n; y++) {
            dst[y + x * n] = src[x + y * n];
        }
    }
}

/* Implement cache blocking below. You should NOT assume that n is a
 * multiple of the block size. */
void transpose_blocking(int n, int blocksize, int *dst, int *src) {
    // YOUR CODE HERE
    int blocks = n / blocksize;
    for(int i = 0; i <= blocks - 1; i++)
        for(int j = 0; j <= blocks - 1; j++)
        for(int a = 0; a < blocksize; a++)
        for(int b = 0; b < blocksize; b++)
        {
            dst[n*(i* blocksize+a) + (j*blocksize+b)] = src[i*blocksize+a + n*(j*blocksize+b)];
        }
    for(int i = 0; i <= blocks - 1; i++)
        for(int a = 0; a < blocksize; a++)
        for(int b = 0; b < n - blocks * blocksize; b++)
        {
            dst[n*(i* blocksize+a) + ((blocks)*blocksize+b)] = src[i*blocksize+a + n*((blocks)*blocksize+b)];
        }
    for(int j = 0; j <= blocks - 1; j++)
        for(int a = 0; a < n - blocks * blocksize; a++)
        for(int b = 0; b < blocksize; b++)
        {
            dst[n*((blocks)* blocksize+a) + (j*blocksize+b)] = src[(blocks)*blocksize+a + n*(j*blocksize+b)];
        }
    for(int a = 0; a < n - blocks * blocksize; a++)
    for(int b = 0; b < n - blocks * blocksize; b++)
    {
        dst[n*((blocks)* blocksize+a) + ((blocks)*blocksize+b)] = src[(blocks)*blocksize+a + n*((blocks)*blocksize+b)];
    }
}

void benchmark(int *A, int *B, int n, int blocksize, 
    void (*transpose)(int, int, int*, int*), char *description) {

    int i, j;
    printf("Testing %s: ", description);

    /* initialize A,B to random integers */
    srand48( time( NULL ) );
    for( i = 0; i < n*n; i++ ) A[i] = lrand48( );
    for( i = 0; i < n*n; i++ ) B[i] = lrand48( );

    /* measure performance */
    struct timeval start, end;

    gettimeofday( &start, NULL );
    transpose( n, blocksize, B, A );
    gettimeofday( &end, NULL );

    double seconds = (end.tv_sec - start.tv_sec) +
        1.0e-6 * (end.tv_usec - start.tv_usec);
    printf( "%g milliseconds\n", seconds*1e3 );


    /* check correctness */
    for( i = 0; i < n; i++ ) {
        for( j = 0; j < n; j++ ) {
            if( B[j+i*n] != A[i+j*n] ) {
                printf("Error!!!! Transpose does not result in correct answer!!\n");
                exit( -1 );
            }
        }
    }
}

int main( int argc, char **argv ) {
    if (argc != 3) {
        printf("Usage: transpose <n> <blocksize>\nExiting.\n");
        exit(1);
    }

    int n = atoi(argv[1]);
    int blocksize = atoi(argv[2]);

    /* allocate an n*n block of integers for the matrices */
    int *A = (int*)malloc( n*n*sizeof(int) );
    int *B = (int*)malloc( n*n*sizeof(int) );

    /* run tests */
    benchmark(A, B, n, blocksize, transpose_naive, "naive transpose");
    benchmark(A, B, n, blocksize, transpose_blocking, "transpose with blocking");

    /* release resources */
    free( A );
    free( B );
    return 0;
}
