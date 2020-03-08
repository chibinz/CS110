#! /bin/sh

gcc -o test.o -c test.c
gcc -o vector.o -c vector.c
ar rcs libvector.a vector.o
gcc -o staticvector test.o libvector.a