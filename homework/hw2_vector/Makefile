CC=gcc
CFLAGS=-Wpedantic -Wall -Wextra -Werror -std=c89

vector: main.o vector.o
	$(CC) $(CFLAGS) -o vector main.o vector.o

vector.o: vector.c vector.h
	$(CC) $(CFLAGS) -c vector.c

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

clean:
	rm vector *.o