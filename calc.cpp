#include <stdlib.h>
#include <numeric>
#include <immintrin.h>
#include <iostream>
#include <omp.h>

constexpr int block = 4;
constexpr int vector_len = 16;
constexpr int chunk = 1024;
constexpr int stride = block * vector_len;

#define action(__i)                                                                                                    \
  btmp = _mm256_loadu_si256((__m256i *)(bs + __i));                                                                    \
  sumv0 = _mm256_add_epi32(_mm256_madd_epi16(_mm256_loadu_si256((__m256i *)(as + __i + k + 0)), btmp), sumv0);         \
  sumv1 = _mm256_add_epi32(_mm256_madd_epi16(_mm256_loadu_si256((__m256i *)(as + __i + k + 1)), btmp), sumv1);         \
  sumv2 = _mm256_add_epi32(_mm256_madd_epi16(_mm256_loadu_si256((__m256i *)(as + __i + k + 2)), btmp), sumv2);         \
  sumv3 = _mm256_add_epi32(_mm256_madd_epi16(_mm256_loadu_si256((__m256i *)(as + __i + k + 3)), btmp), sumv3)

void calc(int n, const int *a, const int *b, int *c)
{
  auto as = (short *)malloc(n * sizeof(short));
  auto bs = (short *)malloc(n * sizeof(short));

#pragma omp parallel for
  for (int i = 0; i < n; i++)
  {
    as[i] = a[i];
    bs[i] = b[i];
  }

#pragma omp parallel for schedule(dynamic)
  for (int k = 0; k < n / block * block; k += block)
  {
    __m256i btmp;


    for(int j = 0; j < (n - k - block) / chunk * chunk; j += chunk)
    {
      auto sumv0 = _mm256_setzero_si256();
      auto sumv1 = _mm256_setzero_si256();
      auto sumv2 = _mm256_setzero_si256();
      auto sumv3 = _mm256_setzero_si256();
      for (int i = j; i < j + chunk; i += stride)
      {
        // Load btmp once, use 4 times
        action(i);
        action(i + vector_len);
        action(i + vector_len * 2);
        action(i + vector_len * 3);
      }
      int sum0[8], sum1[8], sum2[8], sum3[8];

      _mm256_storeu_si256((__m256i*) sum0, sumv0);
      _mm256_storeu_si256((__m256i*) sum1, sumv1);
      _mm256_storeu_si256((__m256i*) sum2, sumv2);
      _mm256_storeu_si256((__m256i*) sum3, sumv3);

      c[k] += sum0[0];
      c[k] += sum0[1];
      c[k] += sum0[2];
      c[k] += sum0[3];
      c[k] += sum0[4];
      c[k] += sum0[5];
      c[k] += sum0[6];
      c[k] += sum0[7];

      c[k + 1] += sum1[0];
      c[k + 1] += sum1[1];
      c[k + 1] += sum1[2];
      c[k + 1] += sum1[3];
      c[k + 1] += sum1[4];
      c[k + 1] += sum1[5];
      c[k + 1] += sum1[6];
      c[k + 1] += sum1[7];

      c[k + 2] += sum2[0];
      c[k + 2] += sum2[1];
      c[k + 2] += sum2[2];
      c[k + 2] += sum2[3];
      c[k + 2] += sum2[4];
      c[k + 2] += sum2[5];
      c[k + 2] += sum2[6];
      c[k + 2] += sum2[7];

      c[k + 3] += sum3[0];
      c[k + 3] += sum3[1];
      c[k + 3] += sum3[2];
      c[k + 3] += sum3[3];
      c[k + 3] += sum3[4];
      c[k + 3] += sum3[5];
      c[k + 3] += sum3[6];
      c[k + 3] += sum3[7];
    }

    auto sumv0 = _mm256_setzero_si256();
    auto sumv1 = _mm256_setzero_si256();
    auto sumv2 = _mm256_setzero_si256();
    auto sumv3 = _mm256_setzero_si256();

    for (int i = (n - k - block) / chunk * chunk; i < (n - k - block) / stride * stride; i += stride)
    {
      action(i);
      action(i + vector_len);
      action(i + vector_len * 2);
      action(i + vector_len * 3);
    }

    int sum0[8], sum1[8], sum2[8], sum3[8];

    _mm256_storeu_si256((__m256i*) sum0, sumv0);
    _mm256_storeu_si256((__m256i*) sum1, sumv1);
    _mm256_storeu_si256((__m256i*) sum2, sumv2);
    _mm256_storeu_si256((__m256i*) sum3, sumv3);

    c[k] += sum0[0];
    c[k] += sum0[1];
    c[k] += sum0[2];
    c[k] += sum0[3];
    c[k] += sum0[4];
    c[k] += sum0[5];
    c[k] += sum0[6];
    c[k] += sum0[7];

    c[k + 1] += sum1[0];
    c[k + 1] += sum1[1];
    c[k + 1] += sum1[2];
    c[k + 1] += sum1[3];
    c[k + 1] += sum1[4];
    c[k + 1] += sum1[5];
    c[k + 1] += sum1[6];
    c[k + 1] += sum1[7];

    c[k + 2] += sum2[0];
    c[k + 2] += sum2[1];
    c[k + 2] += sum2[2];
    c[k + 2] += sum2[3];
    c[k + 2] += sum2[4];
    c[k + 2] += sum2[5];
    c[k + 2] += sum2[6];
    c[k + 2] += sum2[7];

    c[k + 3] += sum3[0];
    c[k + 3] += sum3[1];
    c[k + 3] += sum3[2];
    c[k + 3] += sum3[3];
    c[k + 3] += sum3[4];
    c[k + 3] += sum3[5];
    c[k + 3] += sum3[6];
    c[k + 3] += sum3[7];

    for (int i = 0; i < block; i++)
      for (int j = (n - k - block) / stride * stride; j < n - k - i; j++)
        c[k + i] += a[j + k + i] * b[j];
  }

  for (int k = n / block * block; k < n; k++)
    for (int i = 0; i < n - k; i++)
      c[k] += a[i + k] * b[i];

  free(as);
  free(bs);
}