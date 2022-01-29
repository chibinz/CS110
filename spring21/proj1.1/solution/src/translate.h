/* This project is based on the MIPS Assembler of CS61C in UC Berkeley.
   The framework of this project is been modified to be suitable for RISC-V
   in CS110 course in ShanghaiTech University by Zhijie Yang in March 2019.
   Updated by Chibin Zhang and Zhe Ye in March 2021.
*/

#ifndef TRANSLATE_H
#define TRANSLATE_H

#include <stdint.h>

/* IMPLEMENT ME - see documentation in translate.c */
unsigned write_pass_one(FILE* output, const char* name, char** args, int num_args);

/* IMPLEMENT ME - see documentation in translate.c */
int translate_inst(FILE* output, const char* name, char** args, size_t num_args,
    uint32_t addr, SymbolTable* symtbl);

/* Helper functions. You may want to IMPLEMENT US. */

int write_rtype(uint8_t opcode, uint8_t funct3, uint8_t funct7, FILE* output, char** args, size_t num_args);

int write_itype(uint8_t opcode, uint8_t funct3, FILE* output, char** args, size_t num_args);

int write_stype(uint8_t opcode, uint8_t funct3, FILE* output, char** args, size_t num_args);

int write_sbtype(uint8_t opcode, uint8_t funct3, FILE* output, char** args, size_t num_args,
	uint32_t addr, SymbolTable* symtbl);

int write_utype(uint8_t opcode, FILE* output, char** args, size_t num_args);

int write_ujtype(uint8_t opcode, FILE* output, char** args, size_t num_args,
	uint32_t addr, SymbolTable* symtbl);

#endif
