#include <chrono>
#include <cstring>
#include <iostream>

extern void calc (int, const int *, const int *, int *);

int
main ()
{
  std::ios_base::sync_with_stdio (false);
  int n;
  std::cin >> n;
  int *a = new int[n];
  int *b = new int[n];
  int *c = new int[n];

  // Initialize the arrays
  std::memset (c, 0, sizeof (int) * n);
  for (int i = 0; i < n; ++i)
    std::cin >> a[i] >> b[i];

  // Start the calculation
  auto start = std::chrono::high_resolution_clock::now ();
  calc (n, a, b, c);
  auto stop = std::chrono::high_resolution_clock::now ();
  std::chrono::duration<double> diff = stop - start;
  std::clog << "Time elapsed: " << diff.count () << '\n';

  // Print the result
  for (int i = 0; i < n; i++)
    std::cout << c[i] << '\n';

  delete[] a;
  delete[] b;
  delete[] c;
  return 0;
}
