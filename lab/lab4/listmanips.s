.data
start_msg:	.asciiz "List before: "
end_msg:	.asciiz "List after: "

.text
main:	jal	create_default_list
	add	s1, a0, zero	# a0 = s1 is head of node list

	#print "list before: "
	la	a1, start_msg
	li	a0, 4
	ecall

	#print the list
	add	a1, s1, zero
	jal	print_list

	# print a newline
	jal	print_newline

	#issue the map call
	add	a1, s1, zero	# load the address of the first node into a1
	# load the address of the function into a2 (check out la)
	la	a2, square

	jal	map

	# print "list after: "
	la	a1, end_msg
	li	a0, 4
	ecall

	#print the list
	add	a1, s1, zero
	jal	print_list
	li	a0, 10
	ecall

map:
	addi	sp, sp, -12
	sw	ra, 0(sp)
	sw	s2, 4(sp)
	sw	s1, 8(sp)

	beq	a1, zero, done	# if we were given a null pointer, we're done.

	add	s1, a1, zero	# save address of this node in s1
	add	s2, a2, zero	# save address of function in s2

	# remember that each node is 8 bytes long: 4 for the value followed by 4 for the pointer to next
	# load the value of the current node into a1
	lw	a1, 0(s1)

	# call the function on that value.
	jalr	s2

	# store the returned value back into the node
	sw	a0, 0(s1)

	# load the address of the next node into a1
	lw	a1, 4(s1)

	# put the address of the function back into a2 to prepare for the recursion
	mv	a2, s2

	#recurse
	call	map

done:
	lw	s1, 8(sp)
	lw	s2, 4(sp)
	lw	ra, 0(sp)
	addi	sp, sp, 12
	jr	ra

square:
	mul	a0 ,a1, a1
	jr	ra

create_default_list:
	addi	sp, sp, -4
	sw	ra, 0(sp)
	li	s1, 0		# pointer to the last node we handled
	li	s2, 0		# number of nodes handled
loop:					#do...
	li	a1, 8
	jal	malloc		# get memory for the next node
	sw	s2, 0(a0)		# node->value = i
	sw	s1, 4(a0)		# node->next = last
	add	s1, zero, a0	# last = node
	addi	s2, s2, 1		# i++
    	addi	t1, s2, -10
	bne	t1, zero, loop	# ... while i!= 10
	lw	ra, 0(sp)
	addi	sp, sp, 4
	jr	ra

print_list:
	bne	a1, zero, printMeAndRecurse
	jr	ra 			# nothing to print
printMeAndRecurse:
	add	t0, a1, zero	# t0 gets current node address
	lw	a1, 0(t0)		# a0 gets value in current node
	li	a0, 1		# preparte for print integer ecall
	ecall
	li	a1, ' '		# a0 gets address of string containing space
	li	a0, 11		# prepare for print string ecall
	ecall
	lw	a1, 4(t0)		# a0 gets address of next node
	j	print_list		# recurse. We don't have to use jal because we already have where we want to return to in ra

print_newline:
	li	a1, '\n'
	li	a0, 11
	ecall
	jr	ra

malloc:
	li	a0, 9
	ecall
	jr	ra
