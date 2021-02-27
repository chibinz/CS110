#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tables.h"
#include "translate.h"
#include "translate_utils.h"

/* This project is based on the MIPS Assembler of CS61C in UC Berkeley.
   The framework of this project is been modified to be suitable for RISC-V
   in CS110 course in ShanghaiTech University by Zhijie Yang in March 2019.
*/

/* Writes instructions during the assembler's first pass to OUTPUT. The case
   for general instructions has already been completed, but you need to write
   code to translate the li, bge and mv pseudoinstructions. Your
   pseudoinstruction expansions should not have any side effects.

   NAME is the name of the instruction, ARGS is an array of the arguments, and
   NUM_ARGS specifies the number of items in ARGS.

   Error checking for regular instructions are done in pass two. However, for
   pseudoinstructions, you must make sure that ARGS contains the correct number
   of arguments. You do NOT need to check whether the registers / label are
   valid, since that will be checked in part two.

   Also for li:
    - make sure that the number is representable by 32 bits. (Hint: the number
        can be both signed or unsigned).
    - if the immediate can fit in the imm field of an addiu instruction, then
        expand li into a single addi instruction. Otherwise, expand it into
        a lui-addi pair.

   And for bge and move:
    - your expansion should use the fewest number of instructions possible.

   venus has slightly different translation rules for li, and it allows numbers
   larger than the largest 32 bit number to be loaded with li. You should follow
   the above rules if venus behaves differently.

   Use fprintf() to write. If writing multiple instructions, make sure that
   each instruction is on a different line.

   Returns the number of instructions written (so 0 if there were any errors).
 */
unsigned write_pass_one(FILE *output, const char *name, char **args,
                        int num_args) {

    if (!output || !name || !args)
        return 0;

    char *sub_args[3];
    char buf[100];
    if (strcmp(name, "li") == 0) {
        /* YOUR CODE HERE */

    } else if (strcmp(name, "bge") == 0) {
        /* YOUR CODE HERE */

    } else if (strcmp(name, "mv") == 0) {
        /* YOUR CODE HERE */

    } else {
        write_inst_string(output, name, args, num_args);
        return 1;
    }
}

/* Writes the instruction in hexadecimal format to OUTPUT during pass #2.

   NAME is the name of the instruction, ARGS is an array of the arguments, and
   NUM_ARGS specifies the number of items in ARGS.

   The symbol table (SYMTBL) is given for any symbols that need to be resolved
   at this step.

   You must perform error checking on all instructions and make sure that their
   arguments are valid. If an instruction is invalid, you should not write
   anything to OUTPUT but simply return -1. venus may be a useful resource for
   this step.

   Note the use of helper functions. Consider writing your own! If the function
   definition comes afterwards, you must declare it first (see translate.h).

   Returns 0 on success and -1 on error.
 */
int translate_inst(FILE *output, const char *name, char **args, size_t num_args,
                   uint32_t addr, SymbolTable *symtbl) {
    if (strcmp(name, "add") == 0)
        return write_rtype(0x33, 0x00, 0x00, output, args, num_args);
    else if (strcmp(name, "or") == 0)
        return write_rtype(0x33, 0x06, 0x00, output, args, num_args);
    else if (strcmp(name, "sll") == 0)
        return write_rtype(0x33, 0x01, 0x00, output, args, num_args);
    else if (strcmp(name, "slt") == 0)
        return write_rtype(0x33, 0x02, 0x00, output, args, num_args);
    else if (strcmp(name, "sltu") == 0)
        return write_rtype(0x33, 0x03, 0x00, output, args, num_args);
    /* YOUR CODE HERE */

    else
        return -1;
}

/* A helper function for writing most R-type instructions. You should use
   translate_reg() to parse registers and write_inst_hex() to write to
   OUTPUT. Both are defined in translate_utils.h.

   This function is INCOMPLETE. Complete the implementation below. You will
   find bitwise operations to be the cleanest way to complete this function.
 */
int write_rtype(uint8_t opcode, uint8_t funct3, uint8_t funct7, FILE *output,
                char **args, size_t num_args) {
    /*  Perhaps perform some error checking?  */

    int rd = translate_reg(args[0]);
    int rs = translate_reg(args[1]);
    int rt = translate_reg(args[2]);

    /* YOUR CODE HERE */
    /* error checking for register ids */

    uint32_t instruction;
    /* generate instruction */

    write_inst_hex(output, instruction);
    return 0;
}

int write_itype(uint8_t opcode, uint8_t funct3, FILE *output, char **args,
                size_t num_args) {
    /* YOUR CODE HERE */

    return -1;
}

int write_stype(uint8_t opcode, uint8_t funct3, FILE *output, char **args,
                size_t num_args) {
    /* YOUR CODE HERE */

    return -1;
}

/* Hint: the way for branch to calculate relative address. e.g. bne
     bne rs rt label
   assume the byte_offset(addr) of label is L,
   current instruction byte_offset(addr) is A
   the relative address I for label satisfy:
     L = A + I
   so the relative addres is
     I = L - A              */
int write_sbtype(uint8_t opcode, uint8_t funct3, FILE *output, char **args,
                 size_t num_args, uint32_t addr, SymbolTable *symtbl) {
    /* YOUR CODE HERE */

    return -1;
}

int write_utype(uint8_t opcode, FILE *output, char **args, size_t num_args) {
    /* YOUR CODE HERE */

    return -1;
}

/* In this project there is no need to relocate labels,
   you may think about the reasons. */
int write_ujtype(uint8_t opcode, FILE *output, char **args, size_t num_args,
                 uint32_t addr, SymbolTable *symtbl) {
    /* YOUR CODE HERE */

    return -1;
}
