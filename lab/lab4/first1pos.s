.data
shouldben1:	.asciiz "Should be -1, and firstposshift and firstposmask returned: "
shouldbe0:	.asciiz "Should be 0 , and firstposshift and firstposmask returned: "
shouldbe16:	.asciiz "Should be 16, and firstposshift and firstposmask returned: "
shouldbe31:	.asciiz "Should be 31, and firstposshift and firstposmask returned: "

.text
main:
	la	a1, shouldbe31
	jal	print_str
	lui	a1, 0x80000	# should be 31
	jal	first1posshift
	mv	a1, a0
	jal	print_int
	jal	print_space

	lui	a1, 0x80000
	jal	first1posmask
	mv	a1, a0
	jal	print_int
	jal	print_newline

	la	a1, shouldbe16
	jal	print_str
	lui	a1, 0x0010	# should be 16
	jal	first1posshift
	mv	a1, a0
	jal	print_int
	jal	print_space

	lui	a1, 0x0010
	jal	first1posmask
	mv	a1, a0
	jal	print_int
	jal	print_newline

	la	a1, shouldbe0
	jal	print_str
	li	a1, 1		# should be 0
	jal	first1posshift
	mv	a1, a0
	jal	print_int
	jal	print_space

	li	a1, 1
	jal	first1posmask
	mv	a1, a0
	jal	print_int
	jal	print_newline

	la	a1, shouldben1
	jal	print_str
	mv	a1, zero		# should be -1
	jal	first1posshift
	mv	a1, a0
	jal	print_int
	jal	print_space

	mv	a1, zero
	jal	first1posmask
	mv	a1, a0
	jal	print_int
	jal	print_newline

	li	a0, 10
	ecall

first1posshift:
	addi	a0, zero, -1
	addi	a2, zero, 32
	mv	a3, zero
L1:
	andi	a4, a1, 1
	beqz	a4, L3
	add	a0, zero, a3
L3:
	srli	a1, a1, 1
	addi	a3, a3, 1
	bne	a3, a2, L1
	ret

first1posmask:
	lui	a3, 524288
	addi	a2, zero, 31
L2:
	and	a0, a3, a1
	bnez	a0, L4
	srli	a3, a3, 1
	addi	a2, a2, -1
	addi	a0, zero, -1
	bne	a2, a0, L2
	ret
L4:
	add	a0, zero, a2
	ret

print_int:
	mv	a1, a0
	li	a0, 1
	ecall
	jr	ra

print_str:
	li	a0, 4
	ecall
	jr	ra

print_space:
	li	a1, ' '
	li	a0, 11
	ecall
	jr	ra

print_newline:
	li	a1, '\n'
	li	a0, 11
	ecall
	jr	ra
