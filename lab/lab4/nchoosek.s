.data
shouldbe0:	.asciiz "Should be 0, and it is: "
shouldbe1:	.asciiz "Should be 1, and it is: "
shouldbe4:	.asciiz "Should be 4, and it is: "
shouldbe6:	.asciiz "Should be 6, and it is: "

.text
main:
	la	a1, shouldbe1
	jal	print_str
	li	a1, 4
	li	a2, 0
	jal	nchoosek		# evaluate C(4,0); should be 1
	mv	a1, a0
	jal	print_int
	jal	print_newline

	la	a1, shouldbe4
	jal	print_str
	li	a1, 4
	li	a2, 1
	jal	nchoosek		# evaluate C(4,1); should be 4
	mv	a1, a0
	jal	print_int
	jal	print_newline

	la	a1, shouldbe6
	jal	print_str
	li	a1, 4
	li	a2, 2
	jal	nchoosek		# evaluate C(4,2); should be 6
	mv	a1, a0
	jal	print_int
	jal	print_newline

	la	a1, shouldbe4
	jal	print_str
	li	a1, 4
	li	a2, 3
	jal	nchoosek		# evaluate C(4,3); should be 4
	mv	a1, a0
	jal	print_int
	jal	print_newline

	la	a1, shouldbe1
	jal	print_str
	li	a1, 4
	li	a2, 4
	jal	nchoosek		# evaluate C(4,4); should be 1
	mv	a1, a0
	jal	print_int
	jal	print_newline

	la	a1, shouldbe0
	jal	print_str
	li	a1, 4
	li	a2, 5
	jal	nchoosek		# evaluate C(4,5); should be 0
	mv	a1, a0
	jal	print_int
	jal	print_newline

	li	a0, 10
	ecall

# calculate C(a1,a2)
nchoosek:
	# prologue
	addi	sp, sp, -16
	sw	s1, 0(sp)
	sw	s2, 4(sp)
	sw	s3, 8(sp)
	sw	ra, 12(sp)

	beq	a2, zero, return1
	beq	a1, a2, return1
	beq	a1, zero, return0
	blt	a1, a2, return0

	addi	a1, a1, -1		# C(n,k) = C(n-1,k) + C(n-1,k-1)
	mv	s1, a1
	mv	s2, a2
	jal	nchoosek
	mv	s3, a0
	mv	a1, s1
	addi	a2, s2, -1
	jal	nchoosek
	add	a0, a0, s3
	j	return
return0:
	mv	a0, zero
	j	return
return1:
	addi	a0, zero, 1

return:
	lw	s1, 0(sp)
	lw	s2, 4(sp)
	lw	s3, 8(sp)
	lw	ra, 12(sp)
	addi	sp, sp, 16
	ret

print_int:
	li	a0, 1
	ecall
	jr	ra

print_str:
	li	a0, 4
	ecall
	jr	ra

print_newline:
	li	a1, '\n'
	li	a0, 11
	ecall
	jr	ra
