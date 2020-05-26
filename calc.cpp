#include <emmintrin.h>
#include <immintrin.h>
#include <iostream>
#include <numeric>
#include <omp.h>

constexpr int vec_width = 16;
constexpr int stride = 1024;

void calc(int n, const int *a, const int *b, int *c)
{
    int nt;
    short *sa = (short *)malloc(sizeof(short) * n);
    short *sb = (short *)malloc(sizeof(short) * n);

#pragma omp parallel
    {
        nt = omp_get_num_threads();
    }

#pragma omp parallel for
    for (int i = 0; i < n; i++)
    {
        sa[i] = a[i];
        sb[i] = b[i];
    }

#pragma omp parallel
    for (int k = omp_get_thread_num(); k < n; k += nt)
    {
        int sum = 0;
        unsigned short suma[16];
        __m256i sumv = _mm256_setzero_si256();

        for (int i = 0; i < (n - k) / stride * stride; i += stride)
        {
            for (int j = i; j < i + stride; j += vec_width)
            {
                sumv = _mm256_add_epi16(
                    sumv, _mm256_mullo_epi16(
                              _mm256_loadu_si256((__m256i *)(sa + j + k)),
                              _mm256_loadu_si256((__m256i *)(sb + j))));
            }

            _mm256_storeu_si256((__m256i *)suma, sumv);
            sum = std::accumulate(suma, suma + vec_width, sum);

            sumv = _mm256_setzero_si256();
        }

        _mm256_storeu_si256((__m256i *)suma, sumv);
        sum = std::accumulate(suma, suma + vec_width, sum);

        for (int i = (n - k) / stride * stride; i < n - k; i++)
            sum += sa[i + k] * sb[i];

        c[k] = sum;
    }

    free(sa);
    free(sb);
}
