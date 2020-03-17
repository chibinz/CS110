CC=clang
CFLAG=-Os

RISCV=--target=riscv32 -march=rv32g

hw3.c.s: hw3.c
	$(CC) $(CFLAG) $(RISCV) -S $< -o $@
	./trim.py $@

clean:
	rm -f *.out hw3.c.s