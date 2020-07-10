#!/bin/bash

# Just run this file and you can test your circ files!
# Make sure your alu.circ, regfile.circ, and mem.circ are in THIS directory, and the tests you want to run are in tests/circ_files

cp mem.circ tests/circ_files
cp alu.circ tests/circ_files
cp regfile.circ tests/circ_files
cp cpu.circ tests/circ_files
cd tests/circ_files
./sanity_test.py
cd ../..
