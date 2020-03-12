gcc -fPIC -o vector.o -c vector.c
gcc -shared -o libvector.so vector.o
gcc test.c -o dynamicvector  -L. -lvector -Wl,-rpath=.