.data
.word 2, 4, 6, 8
n: .word 9

.text
main:   add     t0, x0, x0
        # t0 = 0
        addi    t1, x0, 1
        # t1 = 1
        la      t3, n
        # t3 points to 9
        lw      t3, 0(t3)
        # t3 = 9
fib:    beq     t3, x0, finish
        add     t2, t1, t0
        # t2 = t1 + t0
        mv      t0, t1
        # t0 = t1
        mv      t1, t2
        # t1 = t2
        addi    t3, t3, -1
        # t3 -= 1
        j       fib
finish: addi    a0, x0, 1
        addi    a1, t0, 0
        ecall # print t0
        addi    a0, x0, 10
        ecall # terminate ecall

