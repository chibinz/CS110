#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

/* You may find implementing this function helpful */
int bitSigner(unsigned int field UNUSED, unsigned int size UNUSED)
{
    int tmp;
    tmp = field >> (size-1);    /* Get the most significant bit */
    if(tmp == 1)       /* If it is 1, it is negative. */
        return field - (1 << size);   /* 2^size - tmp makes it be the corresponding negative number. */
    else{
        return field;
    }
}

/* Get the immediate of the typical I-TYPE instruction. */
int get_imm_operand(Instruction inst UNUSED)
{
    if(inst.opcode.opcode == 19 && inst.itype.funct3 == 5 && inst.itype.imm >> 5 == 32) /* This is srai. */
        return inst.itype.imm % 32;
    else
        return bitSigner(inst.itype.imm, 12);
}

/* Remember that the offsets should return the offset in BYTES */

int get_branch_offset(Instruction inst UNUSED)  /* Needs to check how the sign functions. */
{
    int tmp = 0;
    tmp += (inst.sbtype.imm5 % 2) << 11;    /* Add 2^11 * imm[11] */
    tmp += (inst.sbtype.imm5 / 2) << 1;     /* Add 2^1 * imm[4:1] */
    tmp += (inst.sbtype.imm7 % 64) << 5;    /* Add 2^5 * imm[10:5] */
    tmp += (inst.sbtype.imm7 / 64) << 12;   /* Add 2^12 * imm[12] */
    return bitSigner(tmp, 13) ;         /* SB has length 13. */
}

int get_jump_offset(Instruction inst UNUSED)
{
    int tmp = 0;
    int tmp2 = inst.ujtype.imm;
    tmp += (inst.ujtype.imm % 256) << 12;   /* Add 2^12 * imm[19:12] */
    tmp2 = tmp2 / 256;
    tmp += (tmp2 % 2) << 11;   /* Add 2^11 * imm[11] */
    tmp2 = tmp2 / 2;
    tmp += (tmp2 % 1024) << 1; /* Add 2^1 * imm[10:1] */
    tmp2 = tmp2 / 1024;
    tmp += (tmp2 % 2) << 20;    /* Add 2^20 * imm[20] */
    return bitSigner(tmp, 21);
}

int get_store_offset(Instruction inst UNUSED)
{
    return bitSigner(inst.stype.imm5 + (inst.stype.imm7 << 5), 12);
}

int get_u_offset(Instruction inst)  /* Need to check */
{
    return inst.utype.imm << 12;
}

void handle_invalid_instruction(Instruction inst)
{
    printf("Invalid Instruction: 0x%08x\n", inst.bits);
}

void handle_invalid_read(Address address)
{
    printf("Bad Read. Address: 0x%08x\n", address);
    exit(-1);
}

void handle_invalid_write(Address address)
{
    printf("Bad Write. Address: 0x%08x\n", address);
    exit(-1);
}
