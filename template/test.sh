#!/bin/bash

cd input

for f in *.s
do
	name=$(echo $f | cut -d. -f1)
	# echo $name
	# INCORRECT
	../assembler $name.s ../out/$name.int ../out/$name.out -log ../log/$name.txt
	if [ -e ../log/ref/${name}_ref.txt ]
	then
		# INCORRECT
		diff ../log/ref/${name}_ref.txt ../log/$name.txt
	else
		diff ../out/ref/${name}_ref.int ../out/$name.int
		diff ../out/ref/${name}_ref.out ../out/$name.out
	fi
done

