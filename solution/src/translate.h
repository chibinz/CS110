#ifndef TRANSLATE_H
#define TRANSLATE_H

#include <stdint.h>

/* IMPLEMENT ME - see documentation in translate.c */
unsigned write_pass_one(FILE *output, const char *name, char **args,
                        int num_args);

/* IMPLEMENT ME - see documentation in translate.c */
int translate_inst(FILE *output, const char *name, char **args, size_t num_args,
                   uint32_t addr, SymbolTable *symtbl, SymbolTable *reltbl);

/* Declaring helper functions: */

int write_rtype(uint8_t opcode, uint8_t funct3, uint8_t funct7, FILE *output,
                char **args, size_t num_args);

/* you may want to IMPLEMENT ME */
int write_itype(uint8_t opcode, uint8_t funct3, FILE *output, char **args,
                size_t num_args);

int write_stype(uint8_t opcode, uint8_t funct3, FILE *output, char **args,
                size_t num_args);

int write_sbtype(uint8_t opcode, uint8_t funct3, FILE *output, char **args,
                 size_t num_args, uint32_t addr, SymbolTable *symtbl);

int write_utype(uint8_t opcode, FILE *output, char **args, size_t num_args);

int write_ujtype(uint8_t opcode, FILE *output, char **args, size_t num_args,
                 uint32_t addr, SymbolTable *reltbl, SymbolTable *symtbl);

#endif
