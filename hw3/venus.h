#pragma once

#define __syscall_one_param(name, type, param, id)                             \
    static void name(type param) {                                             \
        asm volatile("mv a1, %0" ::"r"(param) : "a1");                         \
        asm volatile("li a0, " #id ::: "a0");                                  \
        asm volatile("ecall");                                                 \
    }

__syscall_one_param(putint, int, num, 1);
__syscall_one_param(putstr, char *, str, 4);
__syscall_one_param(putchar, char, ch, 11);
__syscall_one_param(puthex, int, num, 34);
__syscall_one_param(exit, int, error_code, 17);
