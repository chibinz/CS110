#pragma once

#define __syscall_one_param(name, __type, param, id)                           \
  static void name(__type param) {                                             \
    asm volatile("mv a1, %0" ::"r"(param));                                    \
    asm volatile("li a0, " #id);                                               \
    asm volatile("ecall");                                                     \
  }

__syscall_one_param(print_int, int, num, 1);
__syscall_one_param(print_string, char *, str, 4);
__syscall_one_param(print_character, char, ch, 11);
__syscall_one_param(print_hex, int, num, 34);
__syscall_one_param(exit, int, error_code, 17);
