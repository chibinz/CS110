#include <stdlib.h>
#include <numeric>
#include <immintrin.h>
#include <iostream>


#ifdef __AVX512BW__

constexpr int stride = 2048;
constexpr int vector_len = 32;
constexpr int quad_len = 4 * vector_len;

void
calc (int n, const int *a, const int *b, int *c)
{
  short *as, *bs;
  as = (short *)malloc(n * sizeof(short));
  bs = (short *)malloc(n * sizeof(short));

  #pragma omp parallel for
  for(int i = 0; i < n; i++)
  {
    as[i] = a[i];
    bs[i] = b[i];
  }

  #pragma omp parallel for schedule(dynamic)
  for(int ci = 0; ci < n / stride * stride; ci += stride)
  {
    for(int bi = 0; bi < (n - ci - stride)/stride * stride; bi += stride)
    {
      for(int cj = ci; cj < ci + stride; cj ++)
      {
        __m512i sumv = _mm512_setzero_si512();
        for(int bj = bi; bj < bi + stride; bj += quad_len)
        {
          sumv = _mm512_add_epi16(
                    sumv, _mm512_mullo_epi16(
                              _mm512_loadu_si512((__m512i *)(as + bj + cj)),
                              _mm512_loadu_si512((__m512i *)(bs + bj))));

          sumv = _mm512_add_epi16(
                    sumv, _mm512_mullo_epi16(
                              _mm512_loadu_si512((__m512i *)(as + vector_len + bj + cj)),
                              _mm512_loadu_si512((__m512i *)(bs + vector_len + bj))));

          sumv = _mm512_add_epi16(
                    sumv, _mm512_mullo_epi16(
                              _mm512_loadu_si512((__m512i *)(as + vector_len * 2 + bj + cj)),
                              _mm512_loadu_si512((__m512i *)(bs + vector_len * 2 + bj))));

          sumv = _mm512_add_epi16(
                    sumv, _mm512_mullo_epi16(
                              _mm512_loadu_si512((__m512i *)(as + vector_len * 3 + bj + cj)),
                              _mm512_loadu_si512((__m512i *)(bs + vector_len * 3+ bj))));
          //c[cj] += as[cj + bj] * bs[bj];
        }
        unsigned short suma[vector_len];
        _mm512_storeu_si512((__m512i *)suma, sumv);
        c[cj] = std::accumulate(suma, suma + vector_len, c[cj]);
      }
    }

    for(int cj = ci; cj < ci + stride; cj ++)
    {
      __m512i sumv = _mm512_setzero_si512();
      for(int bj = (n - stride - ci)/ stride * stride; bj < (n - cj) / quad_len * quad_len; bj += quad_len)
      {
        sumv = _mm512_add_epi16(
                  sumv, _mm512_mullo_epi16(
                            _mm512_loadu_si512((__m512i *)(as + bj + cj)),
                            _mm512_loadu_si512((__m512i *)(bs + bj))));

        sumv = _mm512_add_epi16(
                  sumv, _mm512_mullo_epi16(
                            _mm512_loadu_si512((__m512i *)(as + vector_len + bj + cj)),
                            _mm512_loadu_si512((__m512i *)(bs + vector_len + bj))));

        sumv = _mm512_add_epi16(
                  sumv, _mm512_mullo_epi16(
                            _mm512_loadu_si512((__m512i *)(as + vector_len * 2 + bj + cj)),
                            _mm512_loadu_si512((__m512i *)(bs + vector_len * 2 + bj))));

        sumv = _mm512_add_epi16(
                  sumv, _mm512_mullo_epi16(
                            _mm512_loadu_si512((__m512i *)(as + vector_len * 3 + bj + cj)),
                            _mm512_loadu_si512((__m512i *)(bs + vector_len * 3+ bj))));
      }
      unsigned short suma[vector_len];
      _mm512_storeu_si512((__m512i *)suma, sumv);
      c[cj] = std::accumulate(suma, suma + vector_len, c[cj]);

      for(int bj = (n - cj) / quad_len * quad_len; bj < n - cj; bj++)
      {
        c[cj] += as[cj + bj] * bs[bj];
      }
    }
  }

  for(int cj = n / stride * stride; cj < n; cj++)
  {
    __m512i sumv = _mm512_setzero_si512();
    for(int bj = 0; bj < (n - cj) / quad_len * quad_len; bj += quad_len)
    {
      sumv = _mm512_add_epi16(
                sumv, _mm512_mullo_epi16(
                          _mm512_loadu_si512((__m512i *)(as + bj + cj)),
                          _mm512_loadu_si512((__m512i *)(bs + bj))));

      sumv = _mm512_add_epi16(
                sumv, _mm512_mullo_epi16(
                          _mm512_loadu_si512((__m512i *)(as + vector_len + bj + cj)),
                          _mm512_loadu_si512((__m512i *)(bs + vector_len + bj))));

      sumv = _mm512_add_epi16(
                sumv, _mm512_mullo_epi16(
                          _mm512_loadu_si512((__m512i *)(as + vector_len * 2 + bj + cj)),
                          _mm512_loadu_si512((__m512i *)(bs + vector_len * 2 + bj))));

      sumv = _mm512_add_epi16(
                sumv, _mm512_mullo_epi16(
                          _mm512_loadu_si512((__m512i *)(as + vector_len * 3 + bj + cj)),
                          _mm512_loadu_si512((__m512i *)(bs + vector_len * 3 + bj))));
    }
    unsigned short suma[vector_len];
    _mm512_storeu_si512((__m512i *)suma, sumv);
    c[cj] = std::accumulate(suma, suma + vector_len, c[cj]);

    for(int bj = (n - cj) / quad_len * quad_len; bj < n - cj; bj++)
    {
      c[cj] += as[cj + bj] * bs[bj];
    }
  }

  free(as);
  free(bs);
}

#else

constexpr int stride = 2048;
constexpr int vector_len = 16;
constexpr int quad_len = 4 * vector_len;

void
calc (int n, const int *a, const int *b, int *c)
{
  short *as, *bs;
  as = (short *)malloc(n * sizeof(short));
  bs = (short *)malloc(n * sizeof(short));

  #pragma omp parallel for
  for(int i = 0; i < n; i++)
  {
    as[i] = a[i];
    bs[i] = b[i];
  }

  #pragma omp parallel for schedule(dynamic)
  for(int ci = 0; ci < n / stride * stride; ci += stride)
  {
    for(int bi = 0; bi < (n - ci - stride)/stride * stride; bi += stride)
    {
      for(int cj = ci; cj < ci + stride; cj ++)
      {
        __m256i sumv = _mm256_setzero_si256();
        for(int bj = bi; bj < bi + stride; bj += quad_len)
        {
          sumv = _mm256_add_epi16(
                    sumv, _mm256_mullo_epi16(
                              _mm256_loadu_si256((__m256i *)(as + bj + cj)),
                              _mm256_loadu_si256((__m256i *)(bs + bj))));

          sumv = _mm256_add_epi16(
                    sumv, _mm256_mullo_epi16(
                              _mm256_loadu_si256((__m256i *)(as + vector_len + bj + cj)),
                              _mm256_loadu_si256((__m256i *)(bs + vector_len + bj))));

          sumv = _mm256_add_epi16(
                    sumv, _mm256_mullo_epi16(
                              _mm256_loadu_si256((__m256i *)(as + vector_len * 2 + bj + cj)),
                              _mm256_loadu_si256((__m256i *)(bs + vector_len * 2 + bj))));

          sumv = _mm256_add_epi16(
                    sumv, _mm256_mullo_epi16(
                              _mm256_loadu_si256((__m256i *)(as + vector_len * 3 + bj + cj)),
                              _mm256_loadu_si256((__m256i *)(bs + vector_len * 3+ bj))));
          //c[cj] += as[cj + bj] * bs[bj];
        }
        unsigned short suma[16];
        _mm256_storeu_si256((__m256i *)suma, sumv);
        c[cj] = std::accumulate(suma, suma + vector_len, c[cj]);
      }
    }

    for(int cj = ci; cj < ci + stride; cj ++)
    {
      __m256i sumv = _mm256_setzero_si256();
      for(int bj = (n - stride - ci)/ stride * stride; bj < (n - cj) / quad_len * quad_len; bj += quad_len)
      {
        sumv = _mm256_add_epi16(
                  sumv, _mm256_mullo_epi16(
                            _mm256_loadu_si256((__m256i *)(as + bj + cj)),
                            _mm256_loadu_si256((__m256i *)(bs + bj))));

        sumv = _mm256_add_epi16(
                  sumv, _mm256_mullo_epi16(
                            _mm256_loadu_si256((__m256i *)(as + vector_len + bj + cj)),
                            _mm256_loadu_si256((__m256i *)(bs + vector_len + bj))));

        sumv = _mm256_add_epi16(
                  sumv, _mm256_mullo_epi16(
                            _mm256_loadu_si256((__m256i *)(as + vector_len * 2 + bj + cj)),
                            _mm256_loadu_si256((__m256i *)(bs + vector_len * 2 + bj))));

        sumv = _mm256_add_epi16(
                  sumv, _mm256_mullo_epi16(
                            _mm256_loadu_si256((__m256i *)(as + vector_len * 3 + bj + cj)),
                            _mm256_loadu_si256((__m256i *)(bs + vector_len * 3+ bj))));
      }
      unsigned short suma[16];
      _mm256_storeu_si256((__m256i *)suma, sumv);
      c[cj] = std::accumulate(suma, suma + vector_len, c[cj]);

      for(int bj = (n - cj) / quad_len * quad_len; bj < n - cj; bj++)
      {
        c[cj] += as[cj + bj] * bs[bj];
      }
    }
  }

  for(int cj = n / stride * stride; cj < n; cj++)
  {
    __m256i sumv = _mm256_setzero_si256();
    for(int bj = 0; bj < (n - cj) / quad_len * quad_len; bj += quad_len)
    {
      sumv = _mm256_add_epi16(
                sumv, _mm256_mullo_epi16(
                          _mm256_loadu_si256((__m256i *)(as + bj + cj)),
                          _mm256_loadu_si256((__m256i *)(bs + bj))));

      sumv = _mm256_add_epi16(
                sumv, _mm256_mullo_epi16(
                          _mm256_loadu_si256((__m256i *)(as + vector_len + bj + cj)),
                          _mm256_loadu_si256((__m256i *)(bs + vector_len + bj))));

      sumv = _mm256_add_epi16(
                sumv, _mm256_mullo_epi16(
                          _mm256_loadu_si256((__m256i *)(as + vector_len * 2 + bj + cj)),
                          _mm256_loadu_si256((__m256i *)(bs + vector_len * 2 + bj))));

      sumv = _mm256_add_epi16(
                sumv, _mm256_mullo_epi16(
                          _mm256_loadu_si256((__m256i *)(as + vector_len * 3 + bj + cj)),
                          _mm256_loadu_si256((__m256i *)(bs + vector_len * 3 + bj))));
    }
    unsigned short suma[16];
    _mm256_storeu_si256((__m256i *)suma, sumv);
    c[cj] = std::accumulate(suma, suma + vector_len, c[cj]);

    for(int bj = (n - cj) / quad_len * quad_len; bj < n - cj; bj++)
    {
      c[cj] += as[cj + bj] * bs[bj];
    }
  }

  free(as);
  free(bs);
}

#endif