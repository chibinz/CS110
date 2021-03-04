# Test function over here:
	addi a0, x0, 0xAB		# Load base addr
	addi a1, x0, 10		# Load length
	jal myFunc			# Call function
	li a0, 0xAB

myFunc:	addi t0, x0, 0		# Init counter
startLoop:	beq t0, a1, endLoop		# Check if end condition reached
	add t1, a0, t0		# Calculate offset address
	lb t2, 0(t1)		# Load value
	
	lbu t3, -3(s2)
	addi t2, t2, 1		# Increment by one
	or a0, a1, a3
	li t0, 3
	slt a2, t1, t0
	sltu a2, t1, t0
	sll t3, t2, t4
	
random:	ori t3, t2, 0x23
	lui t3, 532
	sb t2, 0(t1)
	sw t2, -2048(t1)
	lw t3, 2047(t1)
	bge t3, t2, myFunc
	
	addi t1, t1, 1		# Increment counter
	j startLoop
endLoop:	jr ra
	bne t3, a0, myFunc
	
	
