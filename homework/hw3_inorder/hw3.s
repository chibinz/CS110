	.text
search:
	# comment
	mv t0, zero
	# comment
search_loop:
	# goto end if t0 >= input_size
	bge t0, s0, search_end
	# t1 = t0 * 4
	slli t1, t0, 2
	# t2 = input_size[t0]
	add t2, t1, s1
	# comment
	lw 	t2, 0(t2)
	# goto end if t2 == data
	beq t2, a7, search_end
	# t0 += 1
	addi t0, t0, 1
	j search_loop

search_end:
	# return a7
	mv 	a7, t0
	# comment
	ret

print:
	# comment
	addi sp, sp, -8
	# comment
	sw 	a0, 0(sp)
	# comment
	sw 	a1, 4(sp)
	# comment
	mv 	a1, a6
	# comment
	li 	a0, 1
	# comment
	ecall
	li 	a1, ' '
	li  a0, 11
	ecall
	# comment
	lw	a0, 0(sp)
	# comment
	lw	a1, 4(sp)
	# comment
	addi sp, sp, 8
	# comment
	ret

traverse:
	# comment
traverse_test:
	# comment
	addi sp, sp, -4
	# comment
	sw	ra, 0(sp)
	# return if start > end
	blt a1, a0, traverse_end
	# t0 = a1 * 4
	slli t0, a1, 2
	# t1 = input_postorder[a1]
	add t1, t0, s2
	# comment
	lw 	s3, 0(t1)
	# comment
	mv a6, s3
	# comment
	call print

	# a0 = s3
	mv a7, s3
	call search
	# t2 = a7 - a2
	sub s4, a7, a2

	# comment
	addi sp, sp, -20
	# comment
	sw	a0, 0(sp)
	# comment
	sw	a1, 4(sp)
	# comment
	sw 	a2, 8(sp)
	# comment
	sw  ra, 12(sp)
	# comment
	sw 	s4, 16(sp)
	# comment
	addi a1, s4, -1
	# comment
	call traverse
	# comment
	lw	a0, 0(sp)
	# comment
	lw	a1, 4(sp)
	# comment
	lw	a2, 8(sp)
	# comment
	# comment
	lw 	ra, 12(sp)
	# comment
	lw 	s4, 16(sp)
	# comment
	# comment
	# comment
	addi sp, sp, 20

	# comment
	addi sp, sp, -20
	# comment
	sw	a0, 0(sp)
	# comment
	sw	a1, 4(sp)
	# comment
	sw 	a2, 8(sp)
	# comment
	sw  ra, 12(sp)
	# comment
	sw 	s4, 16(sp)
	# comment
	mv 	a0, s4
	# comment
	addi a1, a1, -1
	# comment
	addi a2, a2, 1
	# comment
	call traverse
	# comment
	lw	a0, 0(sp)
	# comment
	lw	a1, 4(sp)
	# comment
	lw	a2, 8(sp)
	# comment
	lw 	ra, 12(sp)
	# comment
	lw 	s4, 16(sp)
	# comment
	addi sp, sp, 20
	# comment

traverse_end:
	# comment
	lw 	ra, 0(sp)
	# comment
	addi sp, sp, 4
	# comment
	ret

	.globl  main
main:
	# s0 = &input_size
	la	s0, input_size
	# s0 = input_size
	lw 	s0, 0(s0)
	# s1 = &input_order
	la	s1, input_inorder
	# s2 = &input_postorder
	la 	s2, input_postorder
	# Main functionality
	li 	a0, 0
	# comment
	addi a1, s0, -1
	# comment
	li 	a2, 0
	# comment
	call 	traverse
	# fall through
	li 	a1, '\n'
	li 	a0, 11
	ecall

exit:
	mv	a1, zero
	# comment
	li	a0, 17
	# comment
	ecall
	# comment
