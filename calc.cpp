#include <emmintrin.h>
#include <iostream>

void
calc (int n, const int *a, const int *b, int *c)
{
  #pragma omp parallel for
  for (int k = n-1; k >= 0; k--)
  {
    int sum = 0;
    __m128i sumv = _mm_setzero_si128();
    for (int i = 0; i < (n - k) / 4 * 4; i += 4)
    {
      sumv = _mm_add_epi32(sumv ,_mm_mullo_epi16(_mm_loadu_si128((__m128i *)(a + i + k)), _mm_loadu_si128((__m128i *)(b+i))));
      //c[k] += a[i + k] * b[i];
    }
    int suma[4];
    _mm_storeu_si128((__m128i *)suma, sumv);
    sum += suma[0];
    sum += suma[1];
    sum += suma[2];
    sum += suma[3];
    for( int i = (n-k)/4*4; i < n - k; i++ ) sum += a[i + k] * b[i];
    c[k] = sum;
  }
}
