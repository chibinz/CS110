#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define REPEAT 100

int array_size = 10000000;

double *
gen_array (int n)
{
  double *array = malloc(sizeof(*array) * n);
  for (int i = 0; i < n; i++)
    array[i] = drand48 ();
  return array;
}

double
dotp_naive (double *x, double *y, int arr_size)
{
  double global_sum = 0.0;
  #pragma omp parallel
  {
    #pragma omp for
    for (int i = 0; i < arr_size; i++)
      #pragma omp critical
      global_sum += x[i] * y[i];
  }
  return global_sum;
}

// EDIT THIS FUNCTION PART 1
double
dotp_manual_optimized (double *x, double *y, int arr_size)
{
  int nt;

  #pragma omp parallel
  {
    nt = omp_get_num_threads();
  }

  double *sums = calloc(nt, sizeof(double));

  #pragma omp parallel
  {
    int tid = omp_get_thread_num();
    double each_sum = 0.0;

    for (int i = tid; i < arr_size; i += nt)
      each_sum += x[i] * y[i];

    sums[tid] = each_sum;
  }

  double sum = 0;
  for(int i = 0; i < nt; i++)
    sum += sums[i];

  free(sums);

  return sum;
}

// EDIT THIS FUNCTION PART 2
double
dotp_reduction_optimized (double *x, double *y, int arr_size)
{
  double global_sum = 0.0;

  #pragma omp parallel for reduction(+:global_sum)
  for (int i = 0; i < arr_size; i++)
    global_sum += x[i] * y[i];

  return global_sum;
}

int main ()
{
  // Generate input vectors
  double *x = gen_array(array_size), *y = gen_array(array_size);
  double start_time, run_time;
  double serial_result, result;
  int ret = 0;

  // calculate result serially
  start_time = omp_get_wtime ();
  for (int j = 0; j < REPEAT; j++)
    {
      serial_result = 0.0;
      for (int i = 0; i < array_size; i++)
        serial_result += x[i] * y[i];
    }
  run_time = omp_get_wtime () - start_time;
  printf ("Naive solution took %f seconds\n", run_time);

  int num_threads = omp_get_max_threads ();
  for (int i = 1; i <= num_threads; i++)
    {
      omp_set_num_threads (i);
      start_time = omp_get_wtime ();

      for (int j = 0; j < REPEAT; j++)
        result = dotp_manual_optimized (x, y, array_size);

      run_time = omp_get_wtime() - start_time;
      printf ("Manual Optimized: %d thread(s) took %f seconds\n", i, run_time);

      // verify result is correct (within some threshold)
      if (fabs (serial_result - result) > 0.001)
        {
          printf("%g %g %g", serial_result, result, serial_result - result);
          printf("Incorrect result!\n");
          ret = -1;
          goto exit;
        }
    }

  for (int i = 1; i <= num_threads; i++)
    {
      omp_set_num_threads (i);
      start_time = omp_get_wtime ();

      for (int j = 0; j < REPEAT; j++)
        result = dotp_reduction_optimized (x, y, array_size);

      run_time = omp_get_wtime () - start_time;
      printf ("Reduction Optimized: %d thread(s) took %f seconds\n", i, run_time);

      // verify result is correct (within some threshold)
      if (fabs (serial_result - result) > 0.001)
        {
          printf("Incorrect result!\n");
          ret = -1;
          goto exit;
        }
    }

  // Only run this once because it's too slow..
  omp_set_num_threads (1);
  start_time = omp_get_wtime ();
  for (int j = 0; j < REPEAT; j++)
    result = dotp_naive (x, y, array_size);
  run_time = omp_get_wtime () - start_time;

  printf ("Naive: %d thread(s) took %f seconds\n", 1, run_time);

 exit:
  free (x);
  free (y);
  return ret;
}
