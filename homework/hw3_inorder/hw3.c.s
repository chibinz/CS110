	.text
	.globl	print                   # -- Begin function print
print:                                  # @print
# %bb.0:
	#APP
	mv a1, a0
	#NO_APP
	#APP
	li a0, 1
	#NO_APP
	#APP
	ecall
	#NO_APP
	ret
.Lfunc_end0:
                                        # -- End function
	.globl	my_exit                 # -- Begin function my_exit
my_exit:                                # @my_exit
# %bb.0:
	#APP
	mv a1, zero
	#NO_APP
	#APP
	li a0, 17
	#NO_APP
	#APP
	ecall
	#NO_APP
	ret
.Lfunc_end1:
                                        # -- End function
	.globl	search                  # -- Begin function search
search:                                 # @search
# %bb.0:
	mv	a1, a0
	addi	a0, zero, -1
	lui	a2, %hi(input_size)
	lw	a2, %lo(input_size)(a2)
	addi	a3, zero, 1
	blt	a2, a3, .LBB2_4
# %bb.1:
	lui	a3, %hi(input_inorder)
	addi	a3, a3, %lo(input_inorder)
	mv	a4, zero
.LBB2_2:                                # =>This Inner Loop Header: Depth=1
	lw	a5, 0(a3)
	beq	a5, a1, .LBB2_5
# %bb.3:                                #   in Loop: Header=BB2_2 Depth=1
	addi	a3, a3, 4
	addi	a4, a4, 1
	blt	a4, a2, .LBB2_2
.LBB2_4:
	ret
.LBB2_5:
	mv	a0, a4
	ret
.Lfunc_end2:
                                        # -- End function
	.globl	traverse                # -- Begin function traverse
traverse:                               # @traverse
# %bb.0:
	addi	sp, sp, -48
	sw	ra, 44(sp)
	sw	s0, 40(sp)
	sw	s1, 36(sp)
	sw	s2, 32(sp)
	sw	s3, 28(sp)
	sw	s4, 24(sp)
	sw	s5, 20(sp)
	sw	s6, 16(sp)
	sw	s7, 12(sp)
	blt	a1, a0, .LBB3_8
# %bb.1:
	mv	s0, a2
	mv	s1, a0
	lui	a0, %hi(input_postorder)
	addi	s3, a0, %lo(input_postorder)
	lui	s4, %hi(input_size)
	addi	s5, zero, 1
	lui	a0, %hi(input_inorder)
	addi	s6, a0, %lo(input_inorder)
.LBB3_2:                                # =>This Loop Header: Depth=1
                                        #     Child Loop BB3_4 Depth 2
	mv	s2, s1
	mv	s7, a1
	slli	a0, a1, 2
	add	a0, a0, s3
	lw	s1, 0(a0)
	mv	a0, s1
	call	print
	addi	a0, zero, -1
	lw	a1, %lo(input_size)(s4)
	blt	a1, s5, .LBB3_7
# %bb.3:                                #   in Loop: Header=BB3_2 Depth=1
	mv	a3, s6
	mv	a2, zero
.LBB3_4:                                #   Parent Loop BB3_2 Depth=1
                                        # =>  This Inner Loop Header: Depth=2
	lw	a4, 0(a3)
	beq	a4, s1, .LBB3_6
# %bb.5:                                #   in Loop: Header=BB3_4 Depth=2
	addi	a3, a3, 4
	addi	a2, a2, 1
	bne	a1, a2, .LBB3_4
	j	.LBB3_7
.LBB3_6:                                #   in Loop: Header=BB3_2 Depth=1
	mv	a0, a2
.LBB3_7:                                #   in Loop: Header=BB3_2 Depth=1
	sub	s1, a0, s0
	addi	a1, s1, -1
	mv	a0, s2
	mv	a2, s0
	call	traverse
	addi	s0, s0, 1
	addi	a1, s7, -1
	blt	s1, s7, .LBB3_2
.LBB3_8:
	lw	s7, 12(sp)
	lw	s6, 16(sp)
	lw	s5, 20(sp)
	lw	s4, 24(sp)
	lw	s3, 28(sp)
	lw	s2, 32(sp)
	lw	s1, 36(sp)
	lw	s0, 40(sp)
	lw	ra, 44(sp)
	addi	sp, sp, 48
	ret
.Lfunc_end3:
                                        # -- End function
	.globl	main                    # -- Begin function main
main:                                   # @main
# %bb.0:
	addi	sp, sp, -16
	sw	ra, 12(sp)
	lui	a0, %hi(input_size)
	lw	a0, %lo(input_size)(a0)
	addi	a1, a0, -1
	mv	a0, zero
	mv	a2, zero
	call	traverse
	call	my_exit
	mv	a0, zero
	lw	ra, 12(sp)
	addi	sp, sp, 16
	ret
.Lfunc_end4:
                                        # -- End function
	.globl	input_size
input_size:
	.word	7                       # 0x7
	.data
	.globl	input_inorder
input_inorder:
	.word	4                       # 0x4
	.word	2                       # 0x2
	.word	5                       # 0x5
	.word	1                       # 0x1
	.word	6                       # 0x6
	.word	3                       # 0x3
	.word	7                       # 0x7
	.globl	input_postorder
input_postorder:
	.word	4                       # 0x4
	.word	5                       # 0x5
	.word	2                       # 0x2
	.word	6                       # 0x6
	.word	7                       # 0x7
	.word	3                       # 0x3
	.word	1                       # 0x1
