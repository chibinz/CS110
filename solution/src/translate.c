#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tables.h"
#include "translate.h"
#include "translate_utils.h"

/* Writes instructions during the assembler's first pass to OUTPUT. The case
   for general instructions has already been completed, but you need to write
   code to translate the li, bge and move pseudoinstructions. Your
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
        expand li into a single addiu instruction. Otherwise, expand it into
        a lui-ori pair.

   And for bge and move:
    - your expansion should use the fewest number of instructions possible.

   MARS has slightly different translation rules for li, and it allows numbers
   larger than the largest 32 bit number to be loaded with li. You should follow
   the above rules if MARS behaves differently.

   Use fprintf() to write. If writing multiple instructions, make sure that
   each instruction is on a different line.

   Returns the number of instructions written (so 0 if there were any errors).
 */
unsigned write_pass_one(FILE *output, const char *name, char **args,
                        int num_args) {
    long int num;
    /*char *sub_args[3];
    char buf[100];*/
    int returncode;
    long int upper;
    long int lower;
    if (!output || !name || !args)
        return 0;
    if (strcmp(name, "li") == 0) {
        /* YOUR CODE HERE */
        if (num_args != 2) {
            return 0;
        }
        /* YOUR CODE HERE */
        returncode = translate_num(&num, args[1], INT32_MIN,
                                   INT32_MAX); /*translate the number*/
        if (returncode == -1 ||
            translate_reg(args[0]) == -1) /*number in unsigned int 32*/
            return 0;
        if (num >= -2048 && num <= 2047) { /*imm is unsigned */
            fprintf(output, "addi %s x0 %ld\n", args[0], num);
            return 1;
        } else {
            upper = (num >> 12) & 0xfffff; /*set other bits zero.*/
            lower = num & 0xfff;
            if (lower > 0x7ff) {
                upper = upper + 1;
                /*lower = lower - ((upper) >> 12);*/
                lower = lower - 0x1000;
            }
            fprintf(output, "lui %s %ld\n", args[0], upper);
            fprintf(output, "addi %s %s %ld\n", args[0], args[0], lower);
            return 2;
        }

    } else if (strcmp(name, "beqz") == 0) {
        /* YOUR CODE HERE */
        if (num_args == 2) {
            if (translate_reg(args[0]) == -1) /*valid register*/
                return 0;
            /*fprintf(output, "slt $at %s %s\n", args[0], args[1]);*/
            fprintf(output, "beq %s x0 %s\n", args[0], args[1]);
            return 1;
        }
        return 0;
    } else if (strcmp(name, "mv") == 0) {
        /* YOUR CODE HERE */
        if (num_args == 2) {
            if (translate_reg(args[0]) == -1 ||
                translate_reg(args[1]) == -1) /*valid register*/
                return 0;
            fprintf(output, "addi %s %s 0\n", args[0], args[1]);
            return 1;
        }
    } else if (strcmp(name, "j") == 0) {
        /* Something abount "j" */
        if (num_args == 1) {
            fprintf(output, "jal zero %s\n", args[0]);
            return 1;
        }

    } else if (strcmp(name, "jr") == 0) {
        /* Something abount "jr" */
        if (num_args == 1) {
            fprintf(output, "jalr zero %s 0\n", args[0]);
            return 1;
        }

    } else if (strcmp(name, "jal") == 0) {
        /* Something abount "jal" */
        if (num_args == 1) {
            fprintf(output, "jal ra %s\n", args[0]);
            return 1;
        }

    } else if (strcmp(name, "jalr") == 0) {
        /* Something abount "jalr" */

    } else {
        write_inst_string(output, name, args, num_args);
        return 1;
    }
    return 0;
}

/* Writes the instruction in hexadecimal format to OUTPUT during pass #2.

   NAME is the name of the instruction, ARGS is an array of the arguments, and
   NUM_ARGS specifies the number of items in ARGS.

   The symbol table (SYMTBL) is given for any symbols that need to be resolved
   at this step. If a symbol should be relocated, it should be added to the
   relocation table (RELTBL), and the fields for that symbol should be set to
   all zeros.

   You must perform error checking on all instructions and make sure that their
   arguments are valid. If an instruction is invalid, you should not write
   anything to OUTPUT but simply return -1. MARS may be a useful resource for
   this step.

   Note the use of helper functions. Consider writing your own! If the function
   definition comes afterwards, you must declare it first (see translate.h).

   Returns 0 on success and -1 on error.
 */
int translate_inst(FILE *output, const char *name, char **args, size_t num_args,
                   uint32_t addr, SymbolTable *symtbl, SymbolTable *reltbl) {
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
    else if (strcmp(name, "addi") == 0)
        return write_itype(0x13, 0x00, output, args, num_args);
    else if (strcmp(name, "ori") == 0)
        return write_itype(0x13, 0x06, output, args, num_args);
    else if (strcmp(name, "lui") == 0)
        return write_utype(0x37, output, args, num_args);
    else if (strcmp(name, "lb") == 0)
        return write_itype(0x03, 0x00, output, args, num_args);
    else if (strcmp(name, "lbu") == 0)
        return write_itype(0x03, 0x04, output, args, num_args);
    else if (strcmp(name, "lw") == 0)
        return write_itype(0x03, 0x02, output, args, num_args);
    else if (strcmp(name, "sb") == 0)
        return write_stype(0x23, 0x00, output, args, num_args);
    else if (strcmp(name, "sw") == 0)
        return write_stype(0x23, 0x02, output, args, num_args);
    else if (strcmp(name, "beq") == 0)
        return write_sbtype(0x63, 0x00, output, args, num_args, addr, symtbl);
    else if (strcmp(name, "bne") == 0)
        return write_sbtype(0x63, 0x01, output, args, num_args, addr, symtbl);
    else if (strcmp(name, "blt") == 0)
        return write_sbtype(0x63, 0x04, output, args, num_args, addr, symtbl);
    else if (strcmp(name, "bge") == 0)
        return write_sbtype(0x63, 0x05, output, args, num_args, addr, symtbl);
    else if (strcmp(name, "jal") == 0)
        return write_ujtype(0x6F, output, args, num_args, addr, reltbl, symtbl);
    else if (strcmp(name, "jalr") == 0)
        return write_itype(0x67, 0x00, output, args, num_args);
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
    int rd, rs1, rs2;
    uint32_t instruction = 0;

    /*Declares before expressions*/
    if (num_args != 3)
        return -1;

    rd = translate_reg(args[0]);
    rs1 = translate_reg(args[1]);
    rs2 = translate_reg(args[2]);

    /* YOUR CODE HERE */
    if (rd < 0 || rd > 31 || rs1 < 0 || rs1 > 31 || rs2 < 0 || rs2 > 31)
        return -1;
    /* error checking for register ids */

    instruction = 0;
    /* generate instruction */
    instruction =
        instruction |
        (rs1 << 15); /*Shifts 15 bits for rs1 and put it into instruction*/
    instruction =
        instruction |
        (rd << 7); /*Shifts 7 bits for rd and put it into instruction*/
    instruction =
        instruction |
        (rs2 << 20); /*Shifts 20 bits for rs2 and put it into instruction*/
    instruction =
        instruction |
        opcode; /*Shifts 0 bits for opcode and put it into instruction*/
    instruction = instruction | (funct3 << 12);
    instruction = instruction | (funct7 << 25);

    write_inst_hex(output, instruction);
    return 0;
}

int write_itype(uint8_t opcode, uint8_t funct3, FILE *output, char **args,
                size_t num_args) {
    /* YOUR CODE HERE */
    int rs1, error, rd;
    long int immediate;
    uint32_t instruction = 0;
    /*printf ("Num of args: %d\n", num_args);*/
    if (num_args != 3)
        return -1;

    rd = translate_reg(args[0]);
    /* Deal with load instructions */
    if (opcode == 0x03) {
        rs1 = translate_reg(args[2]);
        error = translate_num(&immediate, args[1], -2048, 2047);
    } else {
        rs1 = translate_reg(args[1]);
        /* The immediate is only 12 bits long. */
        error = translate_num(&immediate, args[2], -2048, 2047);
    }
    /*printf ("rs1: %d, error: %d\n", rs1, error);*/
    if (rs1 < 0 || rs1 > 31 || error < 0)
        return -1;
    instruction = instruction | opcode;
    instruction = instruction | (rd << 7);
    instruction = instruction | (funct3 << 12);
    instruction = instruction | (rs1 << 15);
    instruction = instruction | (immediate << 20);
    write_inst_hex(output, instruction);
    return 0;
}

int write_stype(uint8_t opcode, uint8_t funct3, FILE *output, char **args,
                size_t num_args) {
    /* YOUR CODE HERE */
    int rs1, rs2, imm4_0 = 0, imm11_5 = 0, error;
    long int immediate;
    uint32_t instruction = 0;

    if (num_args != 3)
        return -1;

    rs1 = translate_reg(args[2]);
    rs2 = translate_reg(args[0]);
    error = translate_num(&immediate, args[1], -2048, 2047);

    if (rs1 < 0 || rs2 < 0 || error < 0 || rs1 > 31 || rs2 > 31)
        return -1; /*If translate_reg or translate_num fails, return -1*/
    immediate = immediate & 0xffff; /*Make sure immediate is 16 bits long*/
    imm4_0 = (immediate & 0x1f); /* Takes the lower 4 bits of the immediate */
    imm11_5 =
        (immediate & 0xfe0) >> 5; /* Takes the rest bits of the immediate */
    /* generate instruction */
    /*printf ("Immediate: %lx, imm4_0: %x\n", immediate, imm4_0);*/
    instruction = instruction | (funct3 << 12);
    instruction =
        instruction |
        (opcode << 0); /*Shifts 0 bits for opcode and puts it into instruction*/
    instruction =
        instruction |
        (rs1 << 15); /*Shifts 15 bits for rs1 and puts it into instruction*/
    instruction =
        instruction |
        (rs2 << 20); /*Shifts 20 bits for rs2 and puts it into instruction*/
    instruction =
        instruction | (imm4_0 << 7); /*Shifts 7 bits for immediate[4:0] */
    instruction =
        instruction | (imm11_5 << 25); /* Shifts 25 bits for immediate[11:5] */
    write_inst_hex(output, instruction);
    return 0;

    /*return -1;*/
}

/* Hint: the way for branch to calculate relative address. e.g. bne
     bne rs1 rs2 label
   assume the byte_offset(addr) of label is L,
   current instruction byte_offset(addr) is A
   the relative address I  for label satisfy:
     L = (A + 4) + I * 4
   so the relative addres is
     I = (L - A - 4) / 4;  */
int write_sbtype(uint8_t opcode, uint8_t funct3, FILE *output, char **args,
                 size_t num_args, uint32_t addr, SymbolTable *symtbl) {
    /* YOUR CODE HERE */
    uint32_t instruction = 0;
    int L, I, rs1, rs2;
    unsigned int imm4_1 = 0, imm10_5 = 0, imm11 = 0, imm12 = 0;
    char *name;
    /*Declares before expressions*/
    if (num_args != 3)
        return -1;                /*Returns -1 if too much or too few args*/
    rs1 = translate_reg(args[0]); /*Translates rs from args[1]*/
    rs2 = translate_reg(args[1]); /*Translates rt from args[1]*/
    name = args[2];
    if (rs1 < 0 || rs2 < 0 || rs1 > 31 || rs2 > 31 || !symtbl)
        return -1; /*If translate_reg or translate_num fails, return -1*/
    L = (int)get_addr_for_symbol(symtbl, name);
    if (L < 0)
        return -1;
    if ((L - addr - 4) % 4 != 0)
        return -1;
    /*if ((L - addr) % 4 != 0)  return -1;*/
    /*I = (int16_t) (L - addr - 4) / 4;*/
    I = (int16_t)(L - addr);
    instruction =
        instruction |
        opcode; /*Shifts 0  bits for opcode and puts it into instruction*/
    instruction =
        instruction |
        (rs1 << 15); /*Shifts 15 bits for rs1 and puts it into instruction*/
    instruction =
        instruction |
        (rs2 << 20); /*Shifts 20 bits for rs2 and puts it into instruction*/
    instruction = instruction | (funct3 << 12);
    /*I = I & 0xffff;*/
    /*printf ("L: %d, I: %d I_hex: %x, addr: %d\n", L, I, I, addr);*/
    /*printf("Shift 4_1: %d\n", (I / (1 << 1) % (1 << (4-1))));*/
    imm4_1 = (I & 0x1e) >> 1;
    imm10_5 = (I & 0x7e0) >> 5;
    imm11 = (I & 0x800) >> 11;
    imm12 = (I & 0x1000) >> 12;
    /*printf ("imm4_1: %x, imm10_5: %x, imm11: %x, imm12: %x\n", imm4_1,
     * imm10_5, imm11, imm12);*/
    /*printf("imm12: %x, imm10_5: %x, imm4_1: %x, imm11: %x\n", imm12, imm10_5,
     * imm4_1, imm11);*/
    instruction = instruction | (imm12 << 31) | (imm10_5 << 25) |
                  (imm4_1 << 8) |
                  (imm11 << 7); /* Appends the immediate segments */
    write_inst_hex(output, instruction);
    return 0;
}

int write_utype(uint8_t opcode, FILE *output, char **args, size_t num_args) {
    /* YOUR CODE HERE */
    uint32_t instruction = 0;
    int rd, error;
    long int immediate;
    /*Declares before expressions*/
    if (num_args != 2)
        return -1;               /*Returns -1 if too much or too few args*/
    rd = translate_reg(args[0]); /*Translates rt from args[1]*/
    error = translate_num(
        &immediate, args[1], 0,
        1 << 20); /*Translates immediate from args[2] and determines it bounds*/
    if (rd < 0 || error < 0 || rd > 31)
        return -1; /*If translate_reg or translate_num fails, return -1*/
    immediate = immediate & 0xfffff; /*Make sure immediate is 20 bits long*/

    /* generate instruction */
    instruction =
        instruction |
        opcode; /*Shifts 0 bits for opcode and puts it into instruction*/
    instruction =
        instruction |
        (rd << 7); /*Shifts 7 bits for rt and puts it into instruction*/
    instruction =
        instruction |
        immediate
            << 12; /*Shifts 12 bits for immediate and puts it into instruction*/
    write_inst_hex(output, instruction);
    return 0;
}

/* Hint: the relocation table should record
   1. the current instruction byte_offset(addr)
   2. the unsolved LABEL in the jump instruction  */
int write_ujtype(uint8_t opcode, FILE *output, char **args, size_t num_args,
                 uint32_t addr, SymbolTable *reltbl, SymbolTable *symtbl) {
    /* YOUR CODE HERE */
    int32_t error = 0, rd, imm20 = 0, imm10_1 = 0, imm11 = 0, imm19_12 = 0, L,
            I;
    uint32_t instruction = 0;
    if (num_args != 2)
        return -1;
    L = get_addr_for_symbol(symtbl, args[1]);
    error = add_to_table(reltbl, args[1], addr);
    rd = translate_reg(args[0]);
    if (error < 0)
        return -1;
    instruction = instruction | opcode;
    instruction = instruction | rd << 7;
    /*printf ("addr: %d, addr_hex: %x\n", addr, addr);*/
    /* Form the imm array */
    /*printf ("L: %d, L_hex: %x\n", L, L);*/
    I = (int16_t)(L - addr);
    /*printf ("I: %d, I_hex: %x\n", I, I);*/
    imm20 = (I & 0x100000) >> 20;
    imm19_12 = (I & 0xff000) >> 12;
    imm11 = (I & 0x800) >> 11;
    imm10_1 = (I & 0x7fe) >> 1;
    instruction = instruction | (imm20 << 31) | (imm10_1 << 21) |
                  (imm11 << 20) |
                  (imm19_12 << 12); /* Appends the immediate segments */
    write_inst_hex(output, instruction);
    return 0;
}
