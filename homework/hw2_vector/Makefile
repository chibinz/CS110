CC=gcc
CFLAGS=-Wpedantic -Wall -Wextra -Werror -std=c89

vector: test.o vector.o vector.h
	$(CC) $(CFLAGS) -o vector test.o vector.o

vector.o: vector.c vector.h
	$(CC) $(CFLAGS) -c vector.c

test.o: test.c
	$(CC) $(CFLAGS) -c test.c

run: vector
	./vector

tar:
	tar -cvf hw2.tar vector.c dynamiclib.sh staticlib.sh

clean:
	rm -f vector staticvector dynamicvector *.o *.a *.so *.tar