.data
test_input: .word 3 6 7 8

.text
main:
	add t0, x0, x0
	addi t1, x0, 4
	la t2, test_input

main_loop:
	beq t0, t1, main_exit
	slli t3, t0, 2
	add t4, t2, t3
	
	# a0 is the function argument
	lw a0, 0(t4)


	# store temporary registers on stack to preserve content
	addi sp, sp, -20
	sw t0, 0(sp)
	sw t1, 4(sp)
	sw t2, 8(sp)
	sw t3, 12(sp)
	sw t4, 16(sp)

	jal ra, factorial

	# restore temporary register values
	lw t0, 0(sp)
	lw t1, 4(sp)
	lw t2, 8(sp)
	lw t3, 12(sp)
	lw t4, 16(sp)
	addi sp, sp, 20

	addi a1, a0, 0
	addi a0, x0, 1
	ecall # Print Result
	addi a1, x0, ' '
	addi a0, x0, 11
	ecall
	
	addi t0, t0, 1
	jal x0, main_loop

main_exit:  
	addi a0, x0, 10
	ecall # Exit

# function factorial(a0: int) -> a0: int
factorial:
	mv	a5, a0
	li	a0, 1
	beq	a5, x0, end

loop:
	mv	 a4, a5
	addi a5, a5, -1
	mul  a0, a4, a0
	bne  a5, x0, loop

end:
	ret