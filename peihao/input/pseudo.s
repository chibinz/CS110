		li a0, 10
label1:	li a1, -2048
		
label2:
		li a2, 800
		li a3, 0xAB
		bge t3, a0, label1
		bge sp, a1, label2
		mv a2, a3
