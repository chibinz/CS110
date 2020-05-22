#include <emmintrin.h>

void
calc (int n, const int *a, const int *b, int *c)
{
  for (int k = 0; k < n; k++)
    for (int i = 0; i < n - k; ++i)
      c[k] += a[i + k] * b[i];

}

