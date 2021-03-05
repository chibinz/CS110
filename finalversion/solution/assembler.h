/* This project is based on the MIPS Assembler of CS61C in UC Berkeley.
   The framework of this project is been modified to be suitable for RISC-V
   in CS110 course in ShanghaiTech University by Zhijie Yang in March 2019.
   Updated by Chibin Zhang and Zhe Ye in March 2021.
*/

#ifndef ASSEMBLER_H
#define ASSEMBLER_H

/*******************************
 * Do Not Modify Code Below
 *******************************/

int assemble(const char* in, const char* out, const char* int_, const char* sym);

int pass_one(FILE *input, FILE* inter, FILE* symtbl);

int pass_two(FILE *inter, FILE* symtbl, FILE* output);

#endif
