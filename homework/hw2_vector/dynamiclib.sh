#! /bin/sh

gcc -o test.o -c test.c
gcc -shared -o libvector.so -fPIC vector.c
gcc -o dynamicvector test.o -L. -lvector