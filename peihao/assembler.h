#ifndef ASSEMBLER_H
#define ASSEMBLER_H

/*******************************
 * Do Not Modify Code Below
 *******************************/

int assemble(const char* out, const char* in, const char* tmp);

int pass_one(FILE *output, FILE* input, SymbolTable* symtbl);

int pass_two(FILE *output, FILE* input, SymbolTable* symtbl);

#endif
