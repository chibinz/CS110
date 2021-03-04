#! /bin/sh

python venusc.py heap.c
echo ".import input.S" > test.S
cat heap.S >> test.S
java -jar venus-jvm-latest.jar test.S
