CC = clang
CFLAGS = -Wall -Wextra -pthread

SRC = main.c dist.c dijkstra.c util.c

all: dist

debug: CFLAGS += -g
debug: CFLAGS += -fsanitize=address,leak,undefined
debug: dist

release: CFLAGS += -O3
release: dist

bench: release
	time ./dist 4 < input/2.in > /dev/null

dist: $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $@

clean:
	$(RM) dist *.o *.out *.data

.PHONY: clean
