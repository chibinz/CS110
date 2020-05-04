CC=gcc
CFLAGS= -std=gnu99

all: sum
sumWOO: sum.c
	$(CC) $(CFLAGS) -o sum sum.c -O0

sumWO: sum.c
	$(CC) $(CFLAGS) -o sum sum.c -O2

sseTest.s : sseTest.c
	$(CC) -S sseTest.c

clean:
	rm -rf sum
