#include <immintrin.h>
#include <iostream>
#include <numeric>
#include <omp.h>
#include <stdlib.h>

#ifdef __AVX512BW__

constexpr int block = 4;
constexpr int vector_len = 64;
constexpr int half_len = vector_len / 2;
constexpr int chunk = 1024;
constexpr int stride = block * vector_len;

#define action(__i)                                                                                                    \
  btmp = _mm512_loadu_si512((__m512i *)(bs + __i));                                                                    \
  sumv0 = _mm512_add_epi16(_mm512_maddubs_epi16(_mm512_loadu_si512((__m512i *)(as + __i + k + 0)), btmp), sumv0);      \
  sumv1 = _mm512_add_epi16(_mm512_maddubs_epi16(_mm512_loadu_si512((__m512i *)(as + __i + k + 1)), btmp), sumv1);      \
  sumv2 = _mm512_add_epi16(_mm512_maddubs_epi16(_mm512_loadu_si512((__m512i *)(as + __i + k + 2)), btmp), sumv2);      \
  sumv3 = _mm512_add_epi16(_mm512_maddubs_epi16(_mm512_loadu_si512((__m512i *)(as + __i + k + 3)), btmp), sumv3)

void calc(int n, const int *a, const int *b, int *c)
{
  auto as = (unsigned char *)malloc(n * sizeof(unsigned char));
  auto bs = (unsigned char *)malloc(n * sizeof(unsigned char));

#pragma omp parallel for
  for (int i = 0; i < n; i++)
  {
    as[i] = a[i];
    bs[i] = b[i];
  }

#pragma omp parallel for schedule(static, 2)
  for (int k = 0; k < n / block * block; k += block)
  {
    __m512i btmp, sumv0, sumv1, sumv2, sumv3;
    unsigned short sum0[half_len], sum1[half_len], sum2[half_len], sum3[half_len];

    for (int j = 0; j < (n - k - block) / chunk * chunk; j += chunk)
    {
      sumv0 = _mm512_setzero_si512();
      sumv1 = _mm512_setzero_si512();
      sumv2 = _mm512_setzero_si512();
      sumv3 = _mm512_setzero_si512();

      for (int i = j; i < j + chunk; i += stride)
      {
        // Load btmp once, use 4 times
        action(i);
        action(i + vector_len);
        action(i + vector_len * 2);
        action(i + vector_len * 3);
      }

      _mm512_storeu_si512((__m512i *)sum0, sumv0);
      _mm512_storeu_si512((__m512i *)sum1, sumv1);
      _mm512_storeu_si512((__m512i *)sum2, sumv2);
      _mm512_storeu_si512((__m512i *)sum3, sumv3);

      for (int i = 0; i < half_len; i++)
      {
        c[k] += sum0[i];
        c[k + 1] += sum1[i];
        c[k + 2] += sum2[i];
        c[k + 3] += sum3[i];
      }
    }

    sumv0 = _mm512_setzero_si512();
    sumv1 = _mm512_setzero_si512();
    sumv2 = _mm512_setzero_si512();
    sumv3 = _mm512_setzero_si512();

    for (int i = (n - k - block) / chunk * chunk; i < (n - k - block) / stride * stride; i += stride)
    {
      action(i);
      action(i + vector_len);
      action(i + vector_len * 2);
      action(i + vector_len * 3);
    }

    _mm512_storeu_si512((__m512i *)sum0, sumv0);
    _mm512_storeu_si512((__m512i *)sum1, sumv1);
    _mm512_storeu_si512((__m512i *)sum2, sumv2);
    _mm512_storeu_si512((__m512i *)sum3, sumv3);

    for (int i = 0; i < half_len; i++)
    {
      c[k] += sum0[i];
      c[k + 1] += sum1[i];
      c[k + 2] += sum2[i];
      c[k + 3] += sum3[i];
    }

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

#else

constexpr int block = 4;
constexpr int vector_len = 32;
constexpr int half_len = vector_len / 2;
constexpr int chunk = 1024;
constexpr int stride = block * vector_len;

#define action(__i)                                                                                                    \
  btmp = _mm256_loadu_si256((__m256i *)(bs + __i));                                                                    \
  sumv0 = _mm256_add_epi16(_mm256_maddubs_epi16(_mm256_loadu_si256((__m256i *)(as + __i + k + 0)), btmp), sumv0);      \
  sumv1 = _mm256_add_epi16(_mm256_maddubs_epi16(_mm256_loadu_si256((__m256i *)(as + __i + k + 1)), btmp), sumv1);      \
  sumv2 = _mm256_add_epi16(_mm256_maddubs_epi16(_mm256_loadu_si256((__m256i *)(as + __i + k + 2)), btmp), sumv2);      \
  sumv3 = _mm256_add_epi16(_mm256_maddubs_epi16(_mm256_loadu_si256((__m256i *)(as + __i + k + 3)), btmp), sumv3)

void calc(int n, const int *a, const int *b, int *c)
{
  auto as = (unsigned char *)malloc(n * sizeof(unsigned char));
  auto bs = (unsigned char *)malloc(n * sizeof(unsigned char));

#pragma omp parallel for
  for (int i = 0; i < n; i++)
  {
    as[i] = a[i];
    bs[i] = b[i];
  }

#pragma omp parallel for schedule(static, 2)
  for (int k = 0; k < n / block * block; k += block)
  {
    __m256i btmp, sumv0, sumv1, sumv2, sumv3;
    unsigned short sum0[half_len], sum1[half_len], sum2[half_len], sum3[half_len];

    for (int j = 0; j < (n - k - block) / chunk * chunk; j += chunk)
    {
      sumv0 = _mm256_setzero_si256();
      sumv1 = _mm256_setzero_si256();
      sumv2 = _mm256_setzero_si256();
      sumv3 = _mm256_setzero_si256();

      for (int i = j; i < j + chunk; i += stride)
      {
        // Load btmp once, use 4 times
        action(i);
        action(i + vector_len);
        action(i + vector_len * 2);
        action(i + vector_len * 3);
      }

      _mm256_storeu_si256((__m256i *)sum0, sumv0);
      _mm256_storeu_si256((__m256i *)sum1, sumv1);
      _mm256_storeu_si256((__m256i *)sum2, sumv2);
      _mm256_storeu_si256((__m256i *)sum3, sumv3);

      for (int i = 0; i < half_len; i++)
      {
        c[k] += sum0[i];
        c[k + 1] += sum1[i];
        c[k + 2] += sum2[i];
        c[k + 3] += sum3[i];
      }
    }

    sumv0 = _mm256_setzero_si256();
    sumv1 = _mm256_setzero_si256();
    sumv2 = _mm256_setzero_si256();
    sumv3 = _mm256_setzero_si256();

    for (int i = (n - k - block) / chunk * chunk; i < (n - k - block) / stride * stride; i += stride)
    {
      action(i);
      action(i + vector_len);
      action(i + vector_len * 2);
      action(i + vector_len * 3);
    }

    _mm256_storeu_si256((__m256i *)sum0, sumv0);
    _mm256_storeu_si256((__m256i *)sum1, sumv1);
    _mm256_storeu_si256((__m256i *)sum2, sumv2);
    _mm256_storeu_si256((__m256i *)sum3, sumv3);

    for (int i = 0; i < half_len; i++)
    {
      c[k] += sum0[i];
      c[k + 1] += sum1[i];
      c[k + 2] += sum2[i];
      c[k + 3] += sum3[i];
    }

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

#endif