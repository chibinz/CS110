.text
# Text section

# ball and pong is use interchangeably in comments

.align	1
.globl	abs
# Get absolute value of an i32
# Take one argument from a0
abs:
	# Function prologue
	# Allocate space on stack
	addi	sp, sp, -32
	# Store s0
	sw	s0, 28(sp)
	addi	s0, sp, 32
	sw	a0, -20(s0)
	lw	a5, -20(s0)
	# Clever way of returning the absolute
	# value of an number without branching.
	# Excerpted from Reverse Engineering for Beginners
	srai	a4, a5, 31
	lw	a5, -20(s0)
	# Negate if negative
	xor	a5, a4, a5
	# +1 if negative
	sub	a5, a5, a4
	mv	a0, a5
	# Function epilogue
	# Restore s0
	lw	s0, 28(sp)
	addi	sp, sp, 32
	jr	ra

.align	1
.globl	sign
# Get sign of an i32
# Take one argument from a0
sign:
	# Function prologue
	# Allocate space on stack
	addi	sp, sp, -32
	sw	s0, 28(sp)
	# Use s0 as base pointer
	addi	s0, sp, 32
	# Store argument on stack
	sw	a0, -20(s0)
	lw	a5, -20(s0)
	# Branch if negative or zero
	blez	a5, .sign_negative
	# Return 1
	li	a5, 1
	j	.sign_end
.sign_negative:
	# Return -1
	li	a5, -1
.sign_end:
	# Move return value to a0
	mv	a0, a5
	# Function epilogue
	lw	s0, 28(sp)
	# Deallocate
	addi	sp, sp, 32
	# Return
	jr	ra
