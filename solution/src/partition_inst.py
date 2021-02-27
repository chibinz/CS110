def inst_seg (instruction: str, upper: int, lower: int):
	return instruction[(31-upper):(32-lower)]


def partition_instruction (instruction: int, type: str):
	inst_bin: str = bin(instruction);
	inst_32 = '0' * (32 - (len (inst_bin) - 2)) + inst_bin[2:];
	print (hex(instruction), inst_32)
	if type == 'R':
		print ("funct7: 31-25    rs2: 24-20   rs1: 19-15  funct3: 14-12  rd: 11-7  opcode: 6-0")
		print ('   ' + inst_seg(inst_32, 31, 25) + '         ' + inst_seg(inst_32, 24, 20) + '        ' + \
			inst_seg(inst_32, 19, 15) + '          ' + inst_seg(inst_32, 14, 12) + '         ' + \
			inst_seg(inst_32, 11, 7) + '     ' + inst_seg(inst_32, 6, 0));
		print ('   ' + inst_seg(inst_32, 31, 25) + '           ' + str(int(inst_seg(inst_32, 24, 20), 2)) + '            ' + \
			str(int(inst_seg(inst_32, 19, 15), 2)) + '            ' + inst_seg(inst_32, 14, 12) + '           ' + \
			str(int(inst_seg(inst_32, 11, 7), 2)) + '       ' + inst_seg(inst_32, 6, 0));
	if type == 'SB':
		print ("imm[12|10:5]     rs2: 24-20   rs1: 19-15  funct3: 14-12  imm[4:1|11]  opcode: 6-0")
		print ('   ' + inst_seg(inst_32, 31, 25) + '         ' + inst_seg(inst_32, 24, 20) + '        ' + \
			inst_seg(inst_32, 19, 15) + '          ' + inst_seg(inst_32, 14, 12) + '         ' + \
			inst_seg(inst_32, 11, 7) + '     ' + inst_seg(inst_32, 6, 0));
		print ('   ' + inst_seg(inst_32, 31, 25) + '           ' + str(int(inst_seg(inst_32, 24, 20), 2)) + '            ' + \
			str(int(inst_seg(inst_32, 19, 15), 2)) + '            ' + inst_seg(inst_32, 14, 12) + '           ' + \
			str(int(inst_seg(inst_32, 11, 7), 2)) + '       ' + inst_seg(inst_32, 6, 0));
		print ("imm = " + inst_seg(inst_32, 31, 31) + inst_seg(inst_32, 7, 7) + inst_seg(inst_32, 30, 25) + inst_seg(inst_32, 11, 8))
	if type == 'UJ':
		print ("imm[20|10:1|11|19:12]     rd: 11-7  opcode: 6-0")
		print (inst_seg(inst_32, 31, 31) + ' ' + inst_seg(inst_32, 30, 21) + ' ' + \
			inst_seg(inst_32, 20, 20) + ' ' + inst_seg(inst_32, 19, 12) + '      ' + \
			inst_seg(inst_32, 11, 7) + '     ' + inst_seg(inst_32, 6, 0));
		print (hex(int (inst_seg(inst_32, 31, 31) + inst_seg(inst_32, 19, 12) + inst_seg(inst_32, 20, 20) + inst_seg(inst_32, 30, 21), 2)) + \
			  '                        ' + str(int(inst_seg(inst_32, 11, 7), 2)) + '       '+ inst_seg(inst_32, 6, 0));

partition_instruction (0xfc7e00E3, "SB")
partition_instruction (0xFC7E50E3, "SB")
partition_instruction (0xfbc0006f, "UJ")
partition_instruction (0xFBDFF06F, "UJ")