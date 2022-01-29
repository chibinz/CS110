	.import input.S
	.text
	.globl	main                    # -- Begin function main
main:                                   # @main
	addi	sp, sp, -16
	sw	s0, 12(sp)
	sw	s1, 8(sp)
	sw	s2, 4(sp)
	sw	s3, 0(sp)
	la	a7, input_len
	lw	a0, 0(a7)
	addi	a6, zero, 1
	blt	a0, a6, .LBB0_29
	mv	s3, zero
	la	t2, input
	addi	t0, zero, -2
	la	a3, heap
	addi	t6, zero, 32
	addi	t3, zero, 10
	addi	t4, zero, -1
	addi	t5, zero, 3
.LBB0_2:                                # =>This Loop Header: Depth=1
                                        #     Child Loop BB0_25 Depth 2
                                        #     Child Loop BB0_7 Depth 2
                                        #     Child Loop BB0_19 Depth 2
	slli	a0, s3, 2
	add	a0, a0, t2
	lw	a5, 0(a0)
	beq	a5, t0, .LBB0_17
	bne	a5, t4, .LBB0_21
	la	s2, len
	lw	a0, 0(s2)
	bnez	a0, .LBB0_6
	mv a1, t4
	li a0, 17
	ecall
.LBB0_6:                                #   in Loop: Header=BB0_2 Depth=1
	la	t1, heap
	lw	a0, 0(t1)
	mv a1, a0
	li a0, 1
	ecall
	mv a1, t3
	li a0, 11
	ecall
	la	s2, len
	lw	a0, 0(s2)
	addi	a0, a0, -1
	slli	a1, a0, 2
	add	a1, a1, a3
	lw	a1, 0(a1)
	mv	a2, zero
	la	s2, len
	sw	a0, 0(s2)
	mv	s0, a3
	mv	a5, zero
.LBB0_7:                                #   Parent Loop BB0_2 Depth=1
                                        # =>  This Inner Loop Header: Depth=2
	sw	a1, 0(s0)
	slli	s1, a2, 1
	ori	s0, s1, 1
	addi	s1, s1, 2
	blt	s0, a0, .LBB0_10
	blt	s1, a0, .LBB0_13
.LBB0_9:                                #   in Loop: Header=BB0_7 Depth=2
	bne	a5, a2, .LBB0_16
	j	.LBB0_28
.LBB0_10:                               #   in Loop: Header=BB0_7 Depth=2
	slli	a5, s0, 2
	add	a5, a5, a3
	lw	a5, 0(a5)
	blt	a5, a1, .LBB0_12
	mv	s0, a2
.LBB0_12:                               #   in Loop: Header=BB0_7 Depth=2
	mv	a5, s0
	bge	s1, a0, .LBB0_9
.LBB0_13:                               #   in Loop: Header=BB0_7 Depth=2
	slli	s0, s1, 2
	add	s0, s0, a3
	lw	s0, 0(s0)
	slli	a4, a5, 2
	add	a4, a4, a3
	lw	a4, 0(a4)
	blt	s0, a4, .LBB0_15
	mv	s1, a5
.LBB0_15:                               #   in Loop: Header=BB0_7 Depth=2
	mv	a5, s1
	beq	a5, a2, .LBB0_28
.LBB0_16:                               #   in Loop: Header=BB0_7 Depth=2
	slli	a4, a5, 2
	add	s0, a4, a3
	lw	a4, 0(s0)
	slli	a2, a2, 2
	add	a2, a2, a3
	sw	a4, 0(a2)
	mv	a2, a5
	j	.LBB0_7
.LBB0_17:                               #   in Loop: Header=BB0_2 Depth=1
	la	s2, len
	lw	a0, 0(s2)
	blt	a0, a6, .LBB0_20
	mv	a5, zero
	mv	s0, a3
.LBB0_19:                               #   Parent Loop BB0_2 Depth=1
                                        # =>  This Inner Loop Header: Depth=2
	lw	a0, 0(s0)
	mv a1, a0
	li a0, 1
	ecall
	mv a1, t6
	li a0, 11
	ecall
	la	s2, len
	lw	a0, 0(s2)
	addi	a5, a5, 1
	addi	s0, s0, 4
	blt	a5, a0, .LBB0_19
.LBB0_20:                               #   in Loop: Header=BB0_2 Depth=1
	mv a1, t3
	li a0, 11
	ecall
	j	.LBB0_28
.LBB0_21:                               #   in Loop: Header=BB0_2 Depth=1
	la	s2, len
	lw	a0, 0(s2)
	bne	a0, t6, .LBB0_23
	mv a1, t4
	li a0, 17
	ecall
	la	s2, len
	lw	a0, 0(s2)
.LBB0_23:                               #   in Loop: Header=BB0_2 Depth=1
	slli	a1, a0, 2
	add	a1, a1, a3
	sw	a5, 0(a1)
	addi	a1, a0, 1
	la	s2, len
	sw	a1, 0(s2)
	blt	a0, a6, .LBB0_28
	addi	a1, a0, -1
	srli	a2, a1, 31
	add	a1, a1, a2
	srai	s0, a1, 1
	slli	a1, s0, 2
	add	a1, a1, a3
.LBB0_25:                               #   Parent Loop BB0_2 Depth=1
                                        # =>  This Inner Loop Header: Depth=2
	lw	s1, 0(a1)
	bge	a5, s1, .LBB0_28
	mv	a2, a0
	slli	a4, a0, 2
	add	a4, a4, a3
	sw	a5, 0(a1)
	sw	s1, 0(a4)
	blt	a0, t5, .LBB0_28
	mv	a0, s0
	lw	a5, 0(a1)
	addi	a1, s0, -1
	srli	a2, a1, 31
	add	a1, a1, a2
	srai	s0, a1, 1
	slli	a1, s0, 2
	la	a2, heap
	add	a1, a1, a2
	j	.LBB0_25
.LBB0_28:                               #   in Loop: Header=BB0_2 Depth=1
	la	a7, input_len
	lw	a0, 0(a7)
	addi	s3, s3, 1
	blt	s3, a0, .LBB0_2
.LBB0_29:
	mv a1, zero
	li a0, 17
	ecall
	mv	a0, zero
	lw	s3, 0(sp)
	lw	s2, 4(sp)
	lw	s1, 8(sp)
	lw	s0, 12(sp)
	addi	sp, sp, 16
	ret
                                        # -- End function
