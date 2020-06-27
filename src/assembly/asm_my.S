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

.align	1
.globl	cpu_tick
# Align cpu player's pad to pong
cpu_tick:
	# Function prologue
	# Allocate space on stack
	addi	sp, sp, -32
	sw	s0, 28(sp)
	# Use s0 as base pointer
	addi	s0, sp, 32
	# Store argument on stack
	sw	a0, -20(s0)
	# Get address of pong
	lui	a5, %hi(pong)
	addi	a5, a5, %lo(pong)
	# a4 = pong.y
	lw	a4, 4(a5)
	lw	a5, -20(s0)
	# a5 = pong.length
	lw	a5, 8(a5)
	# Divide a5 by 2 without using div
	srli	a3, a5, 31
	add	a5, a3, a5
	srai	a5, a5, 1
	# Negate
	neg	a5, a5
	add	a4, a4, a5
	lw	a5, -20(s0)
	sw	a4, 4(a5)
	# Function epilogue
	lw	s0, 28(sp)
	# Deallocate
	addi	sp, sp, 32
	# Return
	jr	ra

.align	1
.globl	pad_tick
# Take address of a pad, i32*
# Take one argment from a0
pad_tick:
	addi	sp, sp, -32
	# Store ra
	sw	ra, 28(sp)
	# Use s0 as base pointer
	sw	s0, 24(sp)
	addi	s0, sp, 32
	sw	a0, -20(s0)
	# Get status of pin A0
	li	a0, 0
	call	Get_Button
	mv	a5, a0
	beqz	a5, .pad_tick_a1
	lw	a5, -20(s0)
	lw	a5, 4(a5)
	addi	a4, a5, 2
	lw	a5, -20(s0)
	sw	a4, 4(a5)
	j	.top
.pad_tick_a1:
	# Get status of pin A1
	li	a0, 1
	call	Get_Button
	mv	a5, a0
	beqz	a5, .top
	lw	a5, -20(s0)
	lw	a5, 4(a5)
	addi	a4, a5, -2
	lw	a5, -20(s0)
	sw	a4, 4(a5)
.top:
	lw	a5, -20(s0)
	lw	a5, 4(a5)
	li	a4, 10
	bge	a5, a4, .bottom
	li	a5, 10
.bottom:
	lw	a4, -20(s0)
	sw	a5, 4(a4)
	lw	a5, -20(s0)
	lw	a4, 4(a5)
	lw	a5, -20(s0)
	lw	a5, 8(a5)
	add	a4, a4, a5
	li	a5, 69
	ble	a4, a5, .pad_tick_middle
	lw	a5, -20(s0)
	lw	a5, 8(a5)
	li	a4, 70
	sub	a5, a4, a5
	j	.pad_tick_end
.pad_tick_middle:
	lw	a5, -20(s0)
	lw	a5, 4(a5)
.pad_tick_end:
	lw	a4, -20(s0)
	sw	a5, 4(a4)
	lw	ra, 28(sp)
	lw	s0, 24(sp)
	# Restore stack pointer
	addi	sp, sp, 32
	# Return
	jr	ra

.align	1
.globl	ball_tick
# Move pong around
# Add velocity to location of pong
# Takes pointer to ball as argument, a0
ball_tick:
	# Function prologue
	# Allocate space on stack
	addi	sp, sp, -32
	# Use s0 as base pointer
	sw	s0, 28(sp)
	addi	s0, sp, 32
	sw	a0, -20(s0)
	lw	a5, -20(s0)
	# load horizontal velocity
	lw	a4, 0(a5)
	lw	a5, -20(s0)
	# load x
	lw	a5, 12(a5)
	# Add horizontal velocity to coordinate
	add	a4, a4, a5
	lw	a5, -20(s0)
	# Store back
	sw	a4, 0(a5)
	lw	a5, -20(s0)
	# load vertical velocity
	lw	a4, 4(a5)
	lw	a5, -20(s0)
	# load y
	lw	a5, 16(a5)
	# Add vertical velocity to coordinate
	add	a4, a4, a5
	lw	a5, -20(s0)
	# Store back
	sw	a4, 4(a5)
	# Function epilogue
	# Restore s0
	lw	s0, 28(sp)
	# Restore stack pointer
	addi	sp, sp, 32
	# Return
	jr	ra

.align	1
.globl	collide_pad
# Check if pong collided with ball
# Takes 2 argument
# a0: pointer to pong
# a1; pointer to pad
collide_pad:
	# Function prologue
	# Allocate space on stack
	addi	sp, sp, -32
	# Store ra
	sw	ra, 28(sp)
	# Use s0 as base pointer
	sw	s0, 24(sp)
	addi	s0, sp, 32
	sw	a0, -20(s0)
	sw	a1, -24(s0)
	lw	a5, -20(s0)
	# load ball y
	lw	a4, 4(a5)
	lw	a5, -24(s0)
	# load pad y
	lw	a5, 4(a5)
	blt	a4, a5, .collide_pad_end
	lw	a5, -20(s0)
	# load ball y
	lw	a4, 4(a5)
	lw	a5, -24(s0)
	# load y
	lw	a3, 4(a5)
	lw	a5, -24(s0)
	# load length
	lw	a5, 8(a5)
	add	a5, a3, a5
	bgt	a4, a5, .collide_pad_end
	lw	a5, -20(s0)
	# load ball x
	lw	a4, 0(a5)
	lw	a5, -24(s0)
	lw	a5, 12(a5)
	addi	a5, a5, 10
	bgt	a4, a5, .L17
	# Get a random number
	call	rand
	mv	a4, a0
	li	a5, 3
	rem	a5, a4, a5
	addi	a4, a5, 1
	lw	a5, -20(s0)
	# Store back
	sw	a4, 12(a5)
	# Get a random number
	call	rand
	mv	a4, a0
	# in range -3 ~ 3
	li	a5, 7
	rem	a5, a4, a5
	addi	a4, a5, -3
	lw	a5, -20(s0)
	# Store back
	sw	a4, 16(a5)
	j	.collide_pad_end
.L17:
	lw	a5, -20(s0)
	lw	a4, 0(a5)
	lw	a5, -20(s0)
	lw	a5, 8(a5)
	add	a4, a4, a5
	li	a5, 149
	ble	a4, a5, .collide_pad_end
	# Get a random number
	call	rand
	mv	a4, a0
	# in range 0 ~ 2
	li	a5, 3
	rem	a5, a4, a5
	not	a4, a5
	lw	a5, -20(s0)
	# Store back
	sw	a4, 12(a5)
	# Get a random number
	call	rand
	mv	a4, a0
	# in range -3 ~ 3
	li	a5, 7
	rem	a5, a4, a5
	addi	a4, a5, -3
	lw	a5, -20(s0)
	# Store back
	sw	a4, 16(a5)
.collide_pad_end:
	# Function epilogue
	# Restore s0, ra
	lw	ra, 28(sp)
	lw	s0, 24(sp)
	# Restore stack pointer
	addi	sp, sp, 32
	# Return
	jr	ra

.align	1
.globl	collide_wall
collide_wall:
	# Function prologue
	# Allocate space on stack
	addi	sp, sp, -32
	# Use s0 as base pointer
	sw	s0, 28(sp)
	addi	s0, sp, 32
	sw	a0, -20(s0)
	lw	a5, -20(s0)
	lw	a4, 4(a5)
	li	a5, 10
	# Branch if exceed top
	ble	a4, a5, .wall_collide
	lw	a5, -20(s0)
	lw	a4, 4(a5)
	# Don't branch if exceed bottom
	li	a5, 69
	ble	a4, a5, .wall_end
.wall_collide:
	lw	a5, -20(s0)
	lw	a5, 16(a5)
	# Negate pong's vertical speed
	neg	a4, a5
	lw	a5, -20(s0)
	sw	a4, 16(a5)
.wall_end:
	# Function epilogue
	# Restore s0
	lw	s0, 28(sp)
	# Restore stack pointer
	addi	sp, sp, 32
	# Return
	jr	ra

.align	1
.globl	init
init:
	# Function prologue
	# Allocate space on stack
	addi	sp, sp, -16
	# Store ra
	sw	ra, 12(sp)
	# Use s0 as base pointer
	sw	s0, 8(sp)
	addi	s0, sp, 16
	# Load address of p1
	lui	a5, %hi(p1)
	addi	a5, a5, %lo(p1)
	# Set p1 x coord to 10
	li	a4, 10
	# Store back
	sw	a4, 0(a5)
	# Load address of p2
	lui	a5, %hi(p2)
	addi	a5, a5, %lo(p2)
	# Set p2 x coord to 150
	li	a4, 150
	# Store back
	sw	a4, 0(a5)
	# Load address of p2
	lui	a5, %hi(p2)
	addi	a5, a5, %lo(p2)
	# Set p2,p1 y coord to 40
	li	a4, 40
	# Store back
	sw	a4, 4(a5)
	# Load address of p2
	lui	a5, %hi(p2)
	addi	a5, a5, %lo(p2)
	# Store back
	lw	a4, 4(a5)
	# Load address of p1
	lui	a5, %hi(p1)
	addi	a5, a5, %lo(p1)
	# Store back
	sw	a4, 4(a5)
	# Load address of p2
	lui	a5, %hi(p2)
	addi	a5, a5, %lo(p2)
	# Set length to 20
	li	a4, 20
	# Store back
	sw	a4, 8(a5)
	# Load address of p2
	lui	a5, %hi(p2)
	addi	a5, a5, %lo(p2)
	lw	a4, 8(a5)
	# Load address of p1
	lui	a5, %hi(p1)
	addi	a5, a5, %lo(p1)
	# Store back
	sw	a4, 8(a5)
	# Load address of p2
	lui	a5, %hi(p2)
	addi	a5, a5, %lo(p2)
	li	a4, 4
	# Store back
	sw	a4, 12(a5)
	# Load address of p2
	lui	a5, %hi(p2)
	addi	a5, a5, %lo(p2)
	lw	a4, 12(a5)
	# Load address of p1
	lui	a5, %hi(p1)
	addi	a5, a5, %lo(p1)
	sw	a4, 12(a5)
	# Load address of pong
	lui	a5, %hi(pong)
	addi	a5, a5, %lo(pong)
	li	a4, 80
	# Store back
	sw	a4, 0(a5)
	# Load address of pong
	lui	a5, %hi(pong)
	addi	a5, a5, %lo(pong)
	li	a4, 40
	# Store back
	sw	a4, 4(a5)
	# Load address of pong
	lui	a5, %hi(pong)
	addi	a5, a5, %lo(pong)
	li	a4, 2
	# Store back
	sw	a4, 8(a5)
	j	.init_end
.gen_velocity:
	call	rand
	mv	a4, a0
	# Get a random number
	# in range -3 ~ 3
	li	a5, 7
	rem	a5, a4, a5
	addi	a4, a5, -3
	# Load address of pong
	lui	a5, %hi(pong)
	addi	a5, a5, %lo(pong)
	# Store back
	sw	a4, 12(a5)
	# Get a random number
	call	rand
	mv	a4, a0
	# in range -3 ~ 3
	li	a5, 7
	rem	a5, a4, a5
	addi	a4, a5, -3
	# Load address of pong
	lui	a5, %hi(pong)
	addi	a5, a5, %lo(pong)
	# Store back
	sw	a4, 16(a5)
.init_end:
	# Load address of pong
	lui	a5, %hi(pong)
	addi	a5, a5, %lo(pong)
	lw	a5, 12(a5)
	# iff velocity is zero
	# generate another
	beqz	a5, .gen_velocity
	# Function epilogue
	# Restore s0, ra
	lw	ra, 12(sp)
	lw	s0, 8(sp)
	# Restore stack pointer
	addi	sp, sp, 16
	# Return
	jr	ra
