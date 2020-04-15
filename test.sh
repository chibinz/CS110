#!/bin/bash

# Just run this file and you can test your circ files!
# Make sure your files are in the directory above this one though!
# Credits to William Huang

cp alu.circ tests/alu_tests
cp regfile.circ tests/regfile_tests
cd tests
rm -rf student_output
mkdir student_output
./test.py
cd ..
