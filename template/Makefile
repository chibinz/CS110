CC = gcc
CFLAGS = -g -std=c89 -Wall
ASSEMBLER_FILES = src/tables.c src/utils.c src/translate_utils.c src/translate.c

all: assembler

assembler: clean
	$(CC) $(CFLAGS) -o assembler assembler.c $(ASSEMBLER_FILES)

clean:
	rm -f *.o assembler test-assembler core
