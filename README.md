# Project 3

## Problem Setup

Given two arrays $`a`$ and $`b`$ of length $`n`$ which contains positive integers less than or equal to 8. We assume that the array index starts at zero, calculate the $`c`$ array given the following formula:
```math
c_k=\sum_{i=0}^{n-k-1}{a_{i+k}b_i},\quad 0 \leq k < n
```

### Input
The first line contains a positive integer $`n`$. There are two positive integers in each of the next $`n`$ lines representing the elements in the two arrays, i.e., the $`(i+2)`$-th line contains $`a_i`$, $`b_i`$. For the given input, $`n \leq 2^{25}`$.

#### Sample Input
```
5
3 1
2 4
1 1
2 4
1 4
```

### Output
$`n`$ lines. The $`(k+1)`$-th line contains one positive number which represents $`c_k`$.

#### Sample Output
```
24
12
10
6
1
```

## Optimization Techniques
The problem is very simple, and your job is to optimize the naive $`O(n^2)`$ algorithm.

### Algorithm
In fact, there is a faster algorithm (see below) to solve this problem. However, this is not the focus of Computer Architecture, so please do not optimize the algorithm and stick to the naive one.

### Multithreading
The first and the easiest approach is to use multithreading to optimize this algorithm. There are hardly any data dependencies so you do not need to consider much about synchronization. However, the computation workload for each $`c_k`$ is imbalanced, you should think about how to balance it.

### SIMD instructions
This algorithm is also a good candidate for SIMD instructions.

### Loop unrolling
Loop unrolling works very well in combination with SIMD instructions for this algorithm, and you should think about it (Hint: there are two kinds of load: one require alignment and one do not).

### Cache Blocking
For each computation of $`c_k`$, we just stride across the data. If $`n`$ is very large, this wastes cache lines and may slow your program. One way to solve this is to reuse the cache line for next loop (e.g. $`c_{k+1}`$).

## Appendix
Actually, there is a faster algorithm to solve this problem.  
We can do a simple transform on $`a`$ and $`c`$. Let $`a^{\prime}_k=a_{n-k-1}`$, $`c^{\prime}_k=c_{n-k-1}`$, the equation becomes
```math
c^{\prime}_k=\sum_{i=0}^{k}{a^{\prime}_ib_{k-i}},\quad 0 \leq k < n
```
This is the first half of polynomial multiplication, and we can use Fast Fourier Transform to solve it in $`O(n\log{n})`$ time.
