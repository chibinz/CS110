/* This project is based on the MIPS Assembler of CS61C in UC Berkeley.
   The framework of this project is been modified to be suitable for RISC-V
   in CS110 course in ShanghaiTech University by Zhijie Yang in March 2019.
   Updated by Chibin Zhang and Zhe Ye in March 2021.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "tables.h"
#include "translate_utils.h"
#include "translate.h"

/* Writes instructions during the assembler's first pass to OUTPUT. The case
   for general instructions has already been completed, but you need to write
   code to translate the li, bge and mv pseudoinstructions. Your pseudoinstruction
   expansions should not have any side effects.

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
unsigned write_pass_one(FILE* output, const char* name, char** args, int num_args) {

    char *sub_args[3];
    char buf[100];
    long imm, uiSec, addSec;

    if (!output || !name || !args)
        return 0;

    if (strcmp(name, "li") == 0) {
        /* YOUR CODE HERE */

        /* check count of arguments */
        if(num_args != 2) return 0;
        /* translate and check input immediate */
        if(translate_num_32(&imm, args[1]) == -1) return 0;

        /* simplify the expansion if the immediate is small enough to be fit with 12 bits.
           The basical rules are as follows:
           1) the immediate is interpreted within [-2048, 2047]
           2) the immediate is interpreted larger than 2047, but actually represents number
              within [-2048, 2047]. For example, 0xFFF is interpreted to 4096 by strtol, however,
              it should denotes -1.  In this case, we only regard them as unsigned integers, and
              only consider if such unsigned integers can fit in 12-bit slots, we simply use an addi
              for expansion. Since the sign extension in the transation stage, here we don't need
              too much worry about the sign.
           */
        if(_INT12_MIN_ <= imm && imm <= _INT12_MAX_){
            sub_args[0] = args[0];
            sub_args[1] = "x0";
            sub_args[2] = args[1];
            write_inst_string(output, "addi", sub_args, 3);
            return 1;
        }else{

            /* Retrieve the last 20 bits of immediate */
            uiSec = ((imm >> 12) & 0xFFFFF);
            /* Retrieve the first 12 bits of immediate */
            addSec = (imm & 0xFFF);

            /* Pre-increment value placed in upper 20 bits, if sign bit will be
               set on immediate in lower 12 bits, due to the sign extension of addi */
            if((addSec & (1UL << 11)) != 0){
                uiSec++;
                sign_extension(&addSec, addSec, 12);
            }

            /* write as decimal format */
            /* sprintf(buf, "%08x\n", uiSec); */
            sprintf(buf, "%u", (unsigned int)uiSec);
            sub_args[0] = args[0];
            sub_args[1] = buf;
            write_inst_string(output, "lui", sub_args, 2);

            /* write as decimal format */
            /* sprintf(buf, "%08x\n", uiSec); */
            sprintf(buf, "%d", (int)addSec);
            sub_args[0] = args[0];
            sub_args[1] = args[0];
            sub_args[2] = buf;
            write_inst_string(output, "addi", sub_args, 3);
            return 2;
        }
    } else if (strcmp(name, "bgez") == 0) {
        /* YOUR CODE HERE */

        /* check count of arguments */
        if(num_args != 2) return 0;

        /* Setup parameters */
        sub_args[0] = args[0];
        sub_args[1] = "x0";
        sub_args[2] = args[1];
        write_inst_string(output, "beq", sub_args, 3);

        return 1;
    } else if (strcmp(name, "mv") == 0) {
        /* YOUR CODE HERE */

        /* check count of arguments */
        if(num_args != 2) return 0;

        /* Setup parameters */
        sub_args[0] = args[0];
        sub_args[1] = args[1];
        sub_args[2] = "0";
        write_inst_string(output, "addi", sub_args, 3);

        return 1;
    }else if (strcmp(name, "j") == 0) {
        /* YOUR CODE HERE */

        /* check count of arguments */
        if(num_args != 1) return 0;

        /* Setup parameters */
        sub_args[0] = "zero";
        sub_args[1] = args[0];
        write_inst_string(output, "jal", sub_args, 2);

        return 1;
    }else if (strcmp(name, "jr") == 0 ) {
        /* YOUR CODE HERE */

        /* check count of arguments */
        if(num_args != 1) return 0;

        /* Setup parameters */
        sub_args[0] = "zero";
        sub_args[1] = args[0];
        sub_args[2] = "0";
        write_inst_string(output, "jalr", sub_args, 3);

        return 1;
    /* jal and jalr can be explicitly specified or use default ra */
    }else if (strcmp(name, "jal") == 0){
        /* If jal only takes one parameters */
        if(num_args == 1) {

            /* Setup parameters */
            sub_args[0] = "ra";
            sub_args[1] = args[0];
            write_inst_string(output, "jal", sub_args, 2);

            return 1;
        /* If jal contains full parameters */
        }else if(num_args == 2){
            write_inst_string(output, name, args, num_args);
        }
    }else if (strcmp(name, "jalr") == 0) {
        /* If jalr only takes one parameters */
        if(num_args == 1){

            /* Setup parameters */
            sub_args[0] = "ra";
            sub_args[1] = args[0];
            write_inst_string(output, "jalr", sub_args, 2);

            return 1;
        /* If jalr contains full parameters */
        }else if(num_args == 2){
            write_inst_string(output, name, args, num_args);
        }
    }else{
        write_inst_string(output, name, args, num_args);
        return 1;
    }
    return 0;
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
int translate_inst(FILE* output, const char* name, char** args, size_t num_args, uint32_t addr, SymbolTable* symtbl) {
    /** R-format */
    /* add */
    if (strcmp(name, "add") == 0)       return write_rtype (0x33, 0x00, 0x00, output, args, num_args);
    /* or */
    else if (strcmp(name, "or") == 0)    return write_rtype (0x33, 0x06, 0x00, output, args, num_args);
    /* sll */
    else if (strcmp(name, "sll") == 0)   return write_rtype (0x33, 0x01, 0x00, output, args, num_args);
    /* slt */
    else if (strcmp(name, "slt") == 0)   return write_rtype (0x33, 0x02, 0x00, output, args, num_args);
    /* sltu */
    else if (strcmp(name, "sltu") == 0)  return write_rtype (0x33, 0x03, 0x00, output, args, num_args);
    /* YOUR CODE HERE */
    /** I-format */
    /* addi */
    else if (strcmp(name, "addi") == 0)  return write_itype(0x13, 0x00, output, args, num_args);
    /* ori */
    else if (strcmp(name, "ori") == 0)  return write_itype(0x13, 0x06, output, args, num_args);
    /* lb */
    else if (strcmp(name, "lb") == 0)  return write_itype(0x03, 0x00, output, args, num_args);
    /* lbu */
    else if (strcmp(name, "lbu") == 0)  return write_itype(0x03, 0x04, output, args, num_args);
    /* lw */
    else if (strcmp(name, "lw") == 0)  return write_itype(0x03, 0x02, output, args, num_args);
    /* jalr */
    else if (strcmp(name, "jalr") == 0)  return write_itype(0x67, 0x00, output, args, num_args);
    /** S-format */
    /* sw */
    else if (strcmp(name, "sw") == 0) return write_stype(0x23, 0x02, output, args, num_args);
    /* sb */
    else if (strcmp(name, "sb") == 0) return write_stype(0x23, 0x00, output, args, num_args);
    /** SB-format */
    /* beq */
    else if (strcmp(name, "beq") == 0) return write_sbtype(0x63, 0x00, output, args, num_args, addr, symtbl);
    /* bne */
    else if (strcmp(name, "bne") == 0) return write_sbtype(0x63, 0x01, output, args, num_args, addr, symtbl);
    /* blt */
    else if (strcmp(name, "blt") == 0) return write_sbtype(0x63, 0x04, output, args, num_args, addr, symtbl);
    /* bge */
    else if (strcmp(name, "bge") == 0) return write_sbtype(0x63, 0x05, output, args, num_args, addr, symtbl);
    /** U-format */
    /* lui */
    else if (strcmp(name, "lui") == 0) return write_utype(0x37, output, args, num_args);
    /** UJ-format */
    /* jal */
    else if (strcmp(name, "jal") == 0) return write_ujtype(0x6f, output, args, num_args, addr, symtbl);
    /** invalid format */
    else                                 return -1;
}

/* A helper function for writing most R-type instructions. You should use
   translate_reg() to parse registers and write_inst_hex() to write to
   OUTPUT. Both are defined in translate_utils.h.

   This function is INCOMPLETE. Complete the implementation below. You will
   find bitwise operations to be the cleanest way to complete this function.
 */
int write_rtype(uint8_t opcode, uint8_t funct3, uint8_t funct7, FILE* output, char** args, size_t num_args) {
    /*  Perhaps perform some error checking? */
    int rd, rs, rt;
    uint32_t instruction;

    /* R-format requires rd rs rt */
    if (num_args != 3) {
        return -1;
    }

    /* destination register */
    rd = translate_reg(args[0]);
    /* source register 1 */
    rs = translate_reg(args[1]);
    /* source register 2 */
    rt = translate_reg(args[2]);

    /* YOUR CODE HERE */
    /* error checking for register ids */
    if (rd == -1 || rs == -1 || rt == -1) {
        return -1;
    }

    /* generate instruction */
    instruction = opcode + (rd << 7) + (funct3 << 12) + (rs << 15) + (rt << 20) + (funct7 << 25);

    write_inst_hex(output, instruction);
    return 0;
}

int write_itype(uint8_t opcode, uint8_t funct3, FILE* output, char** args, size_t num_args) {
    /* YOUR CODE HERE */
    int rd, rs, imm_ok;
    long imm;
    uint32_t instruction;

    /* I-format requires rd rs imm */
    if (num_args != 3) {
        return -1;
    }

    /* destination register */
    rd = translate_reg(args[0]);
    if (opcode == 0x03) {
        /* source register */
        rs = translate_reg(args[2]);
        /* immediate */
        imm_ok = translate_num_12(&imm, args[1]);
    } else {
        /* source register */
        rs = translate_reg(args[1]);
        /* immediate */
        imm_ok = translate_num_12(&imm, args[2]);
    }

    /* error checking for register ids and number validation */
    if (rd == -1 || rs == -1 || imm_ok == -1) {
        return -1;
    }

    /* generate instruction */
    instruction = opcode + (rd << 7) + (funct3 << 12) + (rs << 15) + (imm << 20);

    write_inst_hex(output, instruction);
    return 0;
}

int write_stype(uint8_t opcode, uint8_t funct3, FILE* output, char** args, size_t num_args) {
    /* YOUR CODE HERE */
    int rs, rt, imm_ok;
    long imm;
    uint32_t instruction;

    /** S-format requires rs rt imm */
    if (num_args != 3) {
        return -1;
    }
    /** source register 1 */
    rs = translate_reg(args[2]);
    /** source register 2 */
    rt = translate_reg(args[0]);
    /** immediate */
    imm_ok = translate_num_12(&imm, args[1]);

    /* error checking for register ids and number validation */
    if (rt == -1 || rs == -1 || imm_ok == -1) {
        return -1;
    }

    /** generate instruction */
    instruction = opcode + ((imm & 0x0000001F) << 7) + (funct3 << 12) + (rs << 15) + (rt << 20) + ((imm >> 5) << 25);

    write_inst_hex(output, instruction);
    return 0;
}

/* Hint: the way for branch to calculate relative address. e.g. bne
     bne rs rt label
   assume the byte_offset(addr) of label is L,
   current instruction byte_offset(addr) is A
   the relative address I for label satisfy:
     L = A + I
   so the relative addres is
     I = L - A              */
int write_sbtype(uint8_t opcode, uint8_t funct3, FILE* output, char** args, size_t num_args,
    uint32_t addr, SymbolTable* symtbl) {
    /* YOUR CODE HERE */
    int rs, rt, imm;
    /** address of the label */
    int64_t label_addr;

    uint32_t instruction;

    /** SB-format requires rs rt imm */
    if (num_args != 3) {
        return -1;
    }

    /** source register 1 */
    rs = translate_reg(args[0]);
    /** source register 2 */
    rt = translate_reg(args[1]);

    /** error checking for label and source registers  */
    if (rs == -1 || rt == -1 || !args[2]) {
        return -1;
    }

    /** get label address */
    label_addr = get_addr_for_symbol(symtbl, args[2]);

    if (label_addr >= 0) {
        /** calculate the offset */
        imm = label_addr - addr;
    }else {  /** if the label is not valid */
        return -1;
    }

    /** if imm is out of range of 12-bits */
    if (imm > _INT12_MAX_ || imm < _INT12_MIN_) {
        return -1;
    }

    /** generate instruction */
    instruction = opcode + (((imm & 0x00000800) >> 11) << 7) + (((imm & 0x0000001E) >> 1) << 8) + (funct3 << 12) + (rs << 15) + (rt << 20) + (((imm & 0x000007E0) >> 5) << 25) + (((imm & 0x00001000) >> 12) << 31);

    write_inst_hex(output, instruction);
    return 0;
}

int write_utype(uint8_t opcode, FILE* output, char** args, size_t num_args) {
    /* YOUR CODE HERE */
    int rd, imm_ok;
    long imm;
    uint32_t instruction;

    /** U-format requires rd imm */
    if (num_args != 2) {
        return -1;
    }

    /** destination register */
    rd = translate_reg(args[0]);
    /** immediate */
    imm_ok = translate_num(&imm, args[1], 0, _UINT20_MAX_);

    /** check validation of rd and imm */
    if (rd == -1 || imm_ok == -1) {
        return -1;
    }

    /** generate instruction */
    instruction = opcode + (rd << 7) + (imm << 12);

    write_inst_hex(output, instruction);
    return 0;
}

/* In this project there is no need to relocate labels,
   you may think about the reasons. */
int write_ujtype(uint8_t opcode, FILE* output, char** args, size_t num_args,
    uint32_t addr, SymbolTable* symtbl) {
    /* YOUR CODE HERE */
    int rd;
    long imm;
    uint32_t instruction;
    /** address of label */
    int64_t label_addr;

    /** UJ-format requires rd imm */
    if (num_args != 2) {
        return -1;
    }

    /** destination register */
    rd = translate_reg(args[0]);

    /** check validation of rd */
    if (rd == -1) {
        return -1;
    }

    /** get label address */
    label_addr = get_addr_for_symbol(symtbl, args[1]);

    if (label_addr >= 0) {
        /** calculate the offset */
        imm = label_addr - addr;
    }else {  /** if the label is not valid */
        return -1;
    }

    /** if imm is out of range of 12-bits */
    if (imm > _INT20_MAX_ || imm < _INT20_MIN_) {
        return -1;
    }

    /** generate instruction */
    instruction = opcode + (rd << 7) + (imm & 0x000FF000) + (((imm & 0x00000800) >> 11) << 20) + (((imm & 0x0000007FE) >> 1) << 21) + (((imm & 0x00100000) >> 20) << 31);

    write_inst_hex(output, instruction);
    return 0;
}
