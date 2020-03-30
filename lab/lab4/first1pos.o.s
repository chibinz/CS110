	.text
	.file	"first1pos.c"
	.globl	first1posshift          # -- Begin function first1posshift
	.p2align	1
	.type	first1posshift,@function
first1posshift:                         # @first1posshift
# %bb.0:
	addi	a0, zero, -1
	addi	a2, zero, 32
	mv	a3, zero
.LBB0_1:                                # =>This Inner Loop Header: Depth=1
	andi	a4, a1, 1
	beqz	a4, .LBB0_3
# %bb.2:                                #   in Loop: Header=BB0_1 Depth=1
	add	a0, zero, a3
.LBB0_3:                                #   in Loop: Header=BB0_1 Depth=1
	srli	a1, a1, 1
	addi	a3, a3, 1
	bne	a3, a2, .LBB0_1
# %bb.4:
	ret
.Lfunc_end0:
	.size	first1posshift, .Lfunc_end0-first1posshift
                                        # -- End function
	.globl	first1posmask           # -- Begin function first1posmask
	.p2align	1
	.type	first1posmask,@function
first1posmask:                          # @first1posmask
# %bb.0:
	lui	a3, 524288
	addi	a2, zero, 31
.LBB1_1:                                # =>This Inner Loop Header: Depth=1
	and	a0, a3, a1
	bnez	a0, .LBB1_4
# %bb.2:                                #   in Loop: Header=BB1_1 Depth=1
	srli	a3, a3, 1
	addi	a2, a2, -1
	addi	a0, zero, -1
	bne	a2, a0, .LBB1_1
# %bb.3:
	ret
.LBB1_4:
	add	a0, zero, a2
	ret
.Lfunc_end1:
	.size	first1posmask, .Lfunc_end1-first1posmask
                                        # -- End function

	.ident	"clang version 9.0.1-8 "
	.section	".note.GNU-stack","",@progbits
	.addrsig
