#!/bin/bash

# Just run this file and you can test your circ files!
# Make sure your alu.circ, regfile.circ, and mem.circ are in THIS directory, and the tests you want to run are in tests/circ_files

cp mem.circ my_tests/circ_files
cp alu.circ my_tests/circ_files
cp regfile.circ my_tests/circ_files
cp cpu.circ my_tests/circ_files
cd my_tests/circ_files
./modular_test.py
cd ../..
