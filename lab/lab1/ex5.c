#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define check_size(t) (printf("Size of %s :  %ld \n", #t, (long int)sizeof(t)))

int main()
{
    check_size(char);
    check_size(short);
    check_size(short int);
    check_size(int);
    check_size(long int);
    check_size(unsigned int);
    check_size(void *);
    check_size(size_t);
    check_size(float);
    check_size(double);
    check_size(int8_t);
    check_size(int16_t);
    check_size(int32_t);
    check_size(int64_t);
    check_size(time_t);
    check_size(clock_t);
    check_size(struct tm);
    check_size(NULL);

    return 1;
}