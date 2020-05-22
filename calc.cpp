#include <emmintrin.h>

void
calc (int n, const int *a, const int *b, int *c)
{
  #pragma omp parallel for
  for (int k = 0; k < n>; k--)
  {
    for (int i = 0; i < (n - k) / 4 * 4; i += 4)
    {
      c[k] += a[i + k] * b[i];
    }
  }
}

