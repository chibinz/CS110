# Homework 3

[Computer Architecture I](../../) [ShanghaiTech University](http://www.shanghaitech.edu.cn/)
[HW2](../2/) HW3 [HW4](../4/)

## Introduction
In this homework, you will implement a binary min heap in RISC-V assembly. Here is a [very simple template](./template.tar) to begin with.

### Input
A reference input is already provided to you in the `input.S` file. Input is provided to you in binary format to alleviate the burden of parsing and dealing with Venus's expermintal file system. There are basically 3 operations that you need to support, `insert`, `pop`, and `print_heap`, described in the comment below. Since this is a computer architecture course, we will not go into the details of how a binary heap works. Refer to your Data Structure course if in doubt.
```S
.data

# Input array
# Non-negative integers are insert operations.
# -1 is the pop operation.
# You should print the popped (smallest) item, while maintaining the heap property.
# -2 is the print heap operation.
# You should the print the internal representation (array) of the heap.
# DO NOT MODIFY THIS VARIABLE
.globl	input
input:
	.word	0 1 2 3 -1 -2

# Constant integer specifying the number of operations
# DO NOT MODIFY THIS VARIABLE
.globl  input_len
input_len:
        .word   6

# Statically allocated array for the heap data structure
# You may assume that the maximum capacity is 32.
# Any operation that causes the array to hold more than 32 elements is an overflow.
.globl	heap
heap:
	.word	0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0

# Global variable that holds the current length of the heap
# You should modify this variable accordingly when inserting or popping elements.
.globl	len
len:
	.word	0
```

You may assume that the input array provided to you contains only **unique integers**.

### Output

It's usually the duty of the supervisor (operating system) to deal with input/output and halting program execution. Venus, being a simple emulator, does not offer us such luxury, but supports a list of primitive [environment calls](https://github.com/ThaumicMekanism/venus/wiki/Environmental-Calls). A snippet of assembly of how to exit with error code 0 is already provided to you in `heap.S`.

In addition handling the forementioned 3 operations correctly, you should also take care of errors. Should a buffer underflow or overflow occur, you must halt execution immediately and exit with error code -1.

In the above case, no error occurs so the output would be
```
0
1 3 2

Exited with error code 0

```

The command that we use to test your program's correctness is
```sh
diff -Z <your_output> <reference_output>
```
The `-Z` flag ignores trailing whitespace, so don't worry about it.

## Running
Make sure that `heap.S` and `input.S` reside in the same directory. To run your program locally
```sh
java -jar venus-jvm-latest.jar heap.S
```

To debug your program online, you might want to replace `.import input.S` in heap.S with the content of input.S.

## Tips

- Handwritten assembly are postfixed with extension `.S` to distinguish from compiler generated assembly `.s`
- While being a strong proponent for python's cult of `space`, I suggest you indent using `tab`s. Set your editor's tabwidth to 8 for best visual experience.
- Learn recursive functions
- Learn save and load from memory using RISC-V
- It is helpful writing in C first
- Write comments

## Execution

We will test your program using RISC-V emulator
[venus](http://autolab.sist.shanghaitech.edu.cn/venus/). You probably want to read
[this](https://github.com/kvakil/venus/wiki) before you started.

## Submission

- You need to follow the [RISC-V integer register convention](https://github.com/riscv/riscv-elf-psabi-doc/blob/master/riscv-elf.md#integer-register-convention-)
  and the [RISC-V integer calling convention](https://github.com/riscv/riscv-elf-psabi-doc/blob/master/riscv-elf.md#-integer-calling-convention).
- You need to have **meaningful** comments not less than 50% of the total lines of code you wrote.
  A comment is defined by a sentence followed by `#`.
- You should submit exactly **one file**, `heap.S`. Any other file found will result in a score of zero.

*****
Last Modified: @DATE@
