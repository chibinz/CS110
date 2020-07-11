#!/usr/bin/env python3

from random import randrange
from math import floor, sqrt

# Maximum number of node
N = 100
# Maximum distance between node
D = floor(sqrt(N))
S = randrange(N)
T = randrange(N)

print(N, S, T)

for i in range(0, N):
    for j in range(0, N):
        if i == j:
            print("0", end = " ")
        else:
            print(randrange(D), end = " ")
    print("")