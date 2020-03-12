gcc -o vector.o -c vector.c
ar rcs libvector.a vector.o
gcc -o staticvector test.c libvector.a