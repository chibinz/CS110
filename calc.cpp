#include <emmintrin.h>
#include <iostream>

void
calc (int n, const int *a, const int *b, int *c)
{
  short *sa, *sb;
  sa = (short *)malloc(sizeof(short) * n);
  sb = (short *)malloc(sizeof(short) * n);

  for(int i = 0; i < n; i++)
  {
    sa[i] = a[i];
    sb[i] = b[i];
  }

  #pragma omp parallel for
  for (int k = n-1; k >= 0; k--)
  {
    unsigned int sum = 0;
    __m128i sumv = _mm_setzero_si128();
    for (int i = 0; i < (n - k) / 8 * 8; i += 8)
    {
      sumv = _mm_add_epi16(sumv ,_mm_mullo_epi16(_mm_loadu_si128((__m128i *)(sa + i + k)), _mm_loadu_si128((__m128i *)(sb+i))));
      //c[k] += a[i + k] * b[i];
    }
    unsigned short suma[8];
    _mm_storeu_si128((__m128i *)suma, sumv);
    sum += suma[0];
    sum += suma[1];
    sum += suma[2];
    sum += suma[3];
    sum += suma[4];
    sum += suma[5];
    sum += suma[6];
    sum += suma[7];
    for( int i = (n-k)/8*8; i < n - k; i++ ) sum += sa[i + k] * sb[i];
    c[k] = sum;
  }
  free(sa);
  free(sb);
}
