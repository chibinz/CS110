reg_list = ['zero', 'ra', 'sp', 'gp', 'tp', 't0', 't1', 't2', 's0', 's1', \
			'a0', 'a1', 'a2', 'a3', 'a4', 'a5', 'a6', 'a7', \
			's2', 's3', 's4', 's5', 's6', 's7', 's8', 's9', 's10', 's11', \
			't3', 't4', 't5', 't6'
			]

print ('if (strcmp(str, "zero") == 0 || strcmp(str, "x0") == 0)      return 0;')
for i in range (1, len (reg_list)):
	# print (i, reg_list[i]);
	print ('else if (strcmp(str, "{}") == 0 || strcmp(str, "x{}") == 0)   return {};'.format (reg_list[i], i, i))
