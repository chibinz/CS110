#!/bin/bash

cd input

for f in *.s
do
	name=$(echo $f | cut -d. -f1)
	if [ $name == "p1_errors" ]
	then
		../assembler -p1 $name.s ../out/$name.int -log ../log/$name.txt
		diff -Z ../log/ref/${name}_ref.txt ../log/$name.txt
	elif [ $name == "p2_errors" ]
	then
		../assembler $name.s ../out/$name.int ../out/$name.out -log ../log/$name.txt
		diff -Z ../log/ref/${name}_ref.txt ../log/$name.txt
	else
		../assembler $name.s ../out/$name.int ../out/$name.out -log ../log/$name.txt
		diff -Z ../out/ref/${name}_ref.int ../out/$name.int
		diff -Z ../out/ref/${name}_ref.out ../out/$name.out
	fi
done
