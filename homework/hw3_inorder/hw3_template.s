# sample input, remeber to remove this before submit
	.data
input_size:
	.word	4
input_inorder:
	.word	1
	.word	2
	.word	3
	.word	4
input_postorder:
	.word	1
	.word	2
	.word	4
	.word	3
# `void exit(int);` system call
	.text
exit:
	mv	a1, a0
	li	a0, 17
	ecall
# entry point
	.globl  main
main:
	li	a0, 0
	call	exit
