void
calc (int n, const int *a, const int *b, int *c)
{
  #pragma omp parallel for
  for (int k = n - 1; k >= 0; k--)
  {
    for (int i = 0; i < n - k; ++i)
    {
      c[k] += a[i + k] * b[i];
    }
  }
}

