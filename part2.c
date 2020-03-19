#include "riscv.h"
#include "types.h"
#include "utils.h"
#include <stdio.h>  /* for stderr */
#include <stdlib.h> /* for exit() */

void execute_rtype(Instruction, Processor *);
void execute_itype_except_load(Instruction, Processor *);
void execute_branch(Instruction, Processor *);
void execute_jal(Instruction, Processor *);
void execute_jalr(Instruction, Processor *);
void execute_load(Instruction, Processor *, Byte *);
void execute_store(Instruction, Processor *, Byte *);
void execute_ecall(Processor *, Byte *);
void execute_utype(Instruction, Processor *);

void execute_instruction(Instruction inst, Processor *p UNUSED, Byte *memory UNUSED)
{
    /* YOUR CODE HERE: COMPLETE THE SWITCH STATEMENTS */
    switch (0)
    { /* What do we switch on? */
        /* YOUR CODE HERE */

    default: /* undefined opcode */
        handle_invalid_instruction(inst);
        exit(-1);
        break;
    }
}

void execute_rtype(Instruction inst, Processor *p UNUSED)
{
    switch (0)
    { /* What do we switch on? */
        /* YOUR CODE HERE */

    default:
        handle_invalid_instruction(inst);
        exit(-1);
        break;
    }
}

void execute_itype_except_load(Instruction inst, Processor *p UNUSED)
{
    int shiftOp;
    shiftOp = -1;
    switch (0)
    { /* What do we switch on? */
        /* YOUR CODE HERE */

    default:
        handle_invalid_instruction(inst);
        exit(-1);
        break;
    }
}

void execute_ecall(Processor *p UNUSED, Byte *memory UNUSED)
{
    switch (0)
    { /* What do we switch on? */
        /* YOUR CODE HERE */

    default:                                     /* undefined ecall */
        printf("Illegal ecall number %d\n", -1); /* What stores the ecall arg? */
        exit(-1);
        break;
    }
}

void execute_branch(Instruction inst, Processor *p UNUSED)
{
    int branchaddr;
    branchaddr = 0;
    /* Remember that the immediate portion of branches
       is counting half-words, so make sure to account for that. */
    switch (0)
    { /* What do we switch on? */
        /* YOUR CODE HERE */

    default:
        handle_invalid_instruction(inst);
        exit(-1);
        break;
    }
}

void execute_load(Instruction inst, Processor *p UNUSED, Byte *memory UNUSED)
{
    switch (0)
    { /* What do we switch on? */
        /* YOUR CODE HERE */

    default:
        handle_invalid_instruction(inst);
        exit(-1);
        break;
    }
}

void execute_store(Instruction inst, Processor *p UNUSED, Byte *memory UNUSED)
{
    switch (0)
    { /* What do we switch on? */
        /* YOUR CODE HERE */

    default:
        handle_invalid_instruction(inst);
        exit(-1);
        break;
    }
}

void execute_jal(Instruction inst UNUSED, Processor *p UNUSED)
{
    Address nextPC;
    nextPC = 0;
    /* YOUR CODE HERE */
}

void execute_jalr(Instruction inst UNUSED, Processor *p UNUSED)
{
    Address nextPC;
    nextPC = 0;
    /* YOUR CODE HERE */
}

void execute_utype(Instruction inst, Processor *p UNUSED)
{
    int imm;
    imm = 0;
    switch (0)
    { /* What do we switch on? */
        /* YOUR CODE HERE */

    default:
        handle_invalid_instruction(inst);
        exit(-1);
        break;
    }
}

/* Checks that the address is aligned correctly */
int check(Address address, Alignment alignment)
{
    if (address > 0 && address < MEMORY_SPACE)
    {
        if (alignment == LENGTH_BYTE)
        {
            return 1;
        }
        else if (alignment == LENGTH_HALF_WORD)
        {
            return address % 2 == 0;
        }
        else if (alignment == LENGTH_WORD)
        {
            return address % 4 == 0;
        }
    }

    return 0;
}

void store(Byte *memory UNUSED, Address address, Alignment alignment, Word value UNUSED, int check_align)
{
    if ((check_align && !check(address, alignment)) || (address >= MEMORY_SPACE))
    {
        handle_invalid_write(address);
    }
    /* YOUR CODE HERE */
}

Word load(Byte *memory UNUSED, Address address, Alignment alignment, int check_align)
{
    Word data = 0; /* initialize our return value to zero */

    if ((check_align && !check(address, alignment)) || (address >= MEMORY_SPACE))
    {
        handle_invalid_read(address);
    }

    /* YOUR CODE HERE */

    return data;
}
