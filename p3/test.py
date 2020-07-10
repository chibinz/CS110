from random import randint;

g = lambda : randint(0, 9)
n = 2 ** 16 - 1

print(n)
for i in range(0, n):
    print(g(), g())