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

#define __vlib_internal(id, param)                                             \
    do {                                                                       \
        asm volatile("mv a1, %0" ::"r"(param) : "a1");                         \
        asm volatile("li a6, " #id ::: "a6");                                  \
        asm volatile("li a0, 0x3cc" ::: "a0");                                 \
        asm volatile("ecall");                                                 \
    } while (0)

static void *malloc(unsigned int size) {
    __vlib_internal(1, size);

    void *retval = 0;
    asm volatile("mv %0, a0" : "=r"(retval));
    return retval;
}

static void *calloc(unsigned int n, unsigned int size) {
    asm volatile("mv a2, %0" ::"r"(size) : "a2");
    __vlib_internal(2, n);

    void *retval = 0;
    asm volatile("mv %0, a0" : "=r"(retval));
    return retval;
}

static void *realloc(void *old, unsigned int size) {
    asm volatile("mv a2, %0" ::"r"(size) : "a2");
    __vlib_internal(3, old);

    void *retval = 0;
    asm volatile("mv %0, a0" : "=r"(retval));
    return retval;
}

static void free(void *ptr) { __vlib_internal(4, ptr); }

static int num_alloc_blocks() {
    int num = 0;
    asm volatile("li a6, 5" ::: "a6");
    asm volatile("li a0, 0x3cc" ::: "a0");
    asm volatile("ecall");
    asm volatile("mv %0, a0" : "=r"(num));
    return num;
}
