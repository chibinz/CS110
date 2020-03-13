# Lab 2

## Exercise 1

- .text is a read-only section containing executable code
- .data is a read-write section containing global or static variables
- .word indicates 32-bit comma separated words (naturally aligned)

- output 34
- fib(9) = 34, The 9th(counting from 0) fibonacci number

- n is stored at 0x10000010, corresponding to register t3

- change t3 to 0xd when pc == 0x14 # fib: beq t3, x0, finish

## Exercise 2
[lab2_ex2_assembly.s](lab2_ex2_assembly.s)
[lab2_ex2_modified.s](lab2_ex2_modified.s)

## Exercise 3
[factorial.s](factorial.s)