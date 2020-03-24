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
    switch (inst.opcode.opcode % 128) /* Find the type of the instructions according to its opcode. */
    {
    case 51: /* 51 = 0x33 Then it is a Rtype. */
        execute_rtype(inst, p);
        break;

    case 3: /* 3 = 0x3 Then it is a load instruction in Itype. */
        execute_load(inst, p, memory);
        break;

    case 19: /* 19 = 0x13 Then it is a Itype. */
        execute_itype_except_load(inst, p);
        break;

    case 107: /* 107 = 0x67 Then it is a jalr. */
        execute_jalr(inst, p);
        break;

    case 115: /* 115 = 0x73 Then it is a ecall */
        execute_ecall(p, memory);
        break;

    case 35: /* 35 = 0x23 Then it is a store instruction. */
        execute_store(inst, p, memory);
        break;

    case 99: /* 99 = 0x63 Then it is a SBtype */
        execute_branch(inst, p);
        break;

    case 23: /* 23 = 0x17 Then it is a utype. */
        execute_utype(inst, p);
        break;

    case 55: /* 55 = 0x37 Then it is a utype. */
        execute_utype(inst, p);
        break;

    case 111: /* 111 = 0x6f Then it is a jal. */
        execute_jal(inst, p);
        break;

    default: /* undefined opcode */
        handle_invalid_instruction(inst);
        exit(-1);
        break;
    }

    p->R[0] = 0; /* set x0 to 0 */
}

void execute_rtype(Instruction inst, Processor *p UNUSED)
{
    int tmp;
    tmp = -1; /* tmp = FFFF FFFF */
    switch (inst.rtype.funct3)
    {
    case 0: /* func3 = 0x0 is add, mul or sub. */
        switch (inst.rtype.funct7)
        {
        case 0: /* func7 = 0x0 is add. */
            p->R[inst.rtype.rd] = p->R[inst.rtype.rs1] + p->R[inst.rtype.rs2];
            break;

        case 1: /* func7 = 0x1 is mul.(low) */
            p->R[inst.rtype.rd] = (p->R[inst.rtype.rs1] * p->R[inst.rtype.rs2]) % (1 << 31);
            break;

        case 32: /* func7 = 0x20 is sub. */
            p->R[inst.rtype.rd] = p->R[inst.rtype.rs1] - p->R[inst.rtype.rs2];
            break;

        default:
            handle_invalid_instruction(inst);
            exit(-1);
            break;
        }
        break;

    case 1:
        switch (inst.rtype.funct7)
        {
        case 0: /* func7 = 0x0 is sll. */
            p->R[inst.rtype.rd] = p->R[inst.rtype.rs1] << p->R[inst.rtype.rs2];
            break;

        case 1: /* func7 = 1 is mulh.(high) */
            p->R[inst.rtype.rd] = (p->R[inst.rtype.rs1] * p->R[inst.rtype.rs2]) / (1 << 31);
            break;

        default:
            handle_invalid_instruction(inst);
            exit(-1);
            break;
        }
        break;

    case 2: /* func3 = 0x2 is slt. */
        p->R[inst.rtype.rd] = (p->R[inst.rtype.rs1] < p->R[inst.rtype.rs2]) ? 1 : 0;
        break;

    case 3: /* func3 = 0x3 is slut. */
        p->R[inst.rtype.rd] = ((unsigned int)p->R[inst.rtype.rs1] < (unsigned int)p->R[inst.rtype.rs2]) ? 1 : 0;
        break;

    case 4:
        switch (inst.rtype.funct7)
        {
        case 0: /* func7 = 0x0 is xor. */
            p->R[inst.rtype.rd] = p->R[inst.rtype.rs1] ^ p->R[inst.rtype.rs2];
            break;

        case 1: /* func7 = 0x1 is div. */
            p->R[inst.rtype.rd] = p->R[inst.rtype.rs1] / p->R[inst.rtype.rs2];
            break;

        default:
            handle_invalid_instruction(inst);
            exit(-1);
            break;
        }
        break;

    case 5:
        switch (inst.rtype.funct7)
        {
        case 0: /* func7 = 0x0 is srl. */
            p->R[inst.rtype.rd] = p->R[inst.rtype.rs1] >> p->R[inst.rtype.rs2];
            break;

        case 32: /* func7 = 0x20 is sra. */
            if (inst.rtype.rs1 < 0)
            {
                p->R[inst.rtype.rd] = (p->R[inst.rtype.rs1] >> p->R[inst.rtype.rs2]) /*+ 1*/;
                p->R[inst.rtype.rd] += tmp << (32 - p->R[inst.rtype.rs2]); /* pad the front part by 1. */
            }
            else
            {
                p->R[inst.rtype.rd] = p->R[inst.rtype.rs1] >> p->R[inst.rtype.rs2];
            }
            break;

        default:
            handle_invalid_instruction(inst);
            exit(-1);
            break;
        }
        break;

    case 6:
        switch (inst.rtype.funct7)
        {
        case 0: /* func7 = 0x0 is or. */
            p->R[inst.rtype.rd] = p->R[inst.rtype.rs1] | p->R[inst.rtype.rs2];
            break;

        case 1: /* func7 = 0x1 is rem. */
            p->R[inst.rtype.rd] = p->R[inst.rtype.rs1] % p->R[inst.rtype.rs2];
            break;

        default:
            handle_invalid_instruction(inst);
            exit(-1);
            break;
        }
        break;

    case 7: /* func3 = 0x7 is and. */
        inst.rtype.rd = inst.rtype.rs1 & inst.rtype.rs2;
        break;

    default:
        handle_invalid_instruction(inst);
        exit(-1);
        break;
    }

    p->PC += 4; /* go to the next instruction. */
}

void execute_itype_except_load(Instruction inst, Processor *p UNUSED)
{
    int shiftOp;
    shiftOp = -1;
    switch (inst.itype.funct3)
    {
    case 0: /* func3 = 0x0 is addi */
        p->R[inst.itype.rd] = p->R[inst.itype.rs1] + get_imm_operand(inst);
        break;

    case 1: /* func3 = 0x1 is slli. */
        p->R[inst.itype.rd] = p->R[inst.itype.rs1] << get_imm_operand(inst);
        break;

    case 2: /* func3 = 0x2 is slti. */
        p->R[inst.itype.rd] = ((int)p->R[inst.itype.rs1] < get_imm_operand(inst)) ? 1 : 0;
        break;

    case 3: /* ........... is sltiu. */
        p->R[inst.itype.rd] = ((unsigned int)p->R[inst.itype.rs1] + (unsigned int)get_imm_operand(inst)) ? 1 : 0;
        break;

    case 4: /* ... is xori.*/
        p->R[inst.itype.rd] = p->R[inst.itype.rs1] ^ get_imm_operand(inst);
        break;

    case 5:
        switch (inst.rtype.funct7)
        {
        case 0: /* ... is srli. */
            p->R[inst.itype.rd] = p->R[inst.itype.rs1] >> get_imm_operand(inst);
            break;

        case 32: /* ... is srai. */
            if (inst.rtype.rs1 < 0)
            {
                p->R[inst.rtype.rd] = (p->R[inst.rtype.rs1] >> get_imm_operand(inst)) /*+ 1*/;
                p->R[inst.rtype.rd] += shiftOp << (32 - get_imm_operand(inst)); /* pad the front part by 1. */
            }
            else
            {
                p->R[inst.rtype.rd] = p->R[inst.rtype.rs1] >> get_imm_operand(inst);
            }
            break;

        default:
            handle_invalid_instruction(inst);
            exit(-1);
            break;
        }
        break;

    case 6: /* .. is ori. */
        p->R[inst.rtype.rd] = p->R[inst.rtype.rs1] | get_imm_operand(inst);
        break;

    case 7: /* ... is andi. */
        p->R[inst.rtype.rd] = p->R[inst.rtype.rs1] & get_imm_operand(inst);
        break;

    default:
        handle_invalid_instruction(inst);
        exit(-1);
        break;
    }

    p->PC += 4; /* go to the next instruction. */
}

void execute_ecall(Processor *p UNUSED, Byte *memory UNUSED)
{
    switch (p->R[10]) /* Determine the funcion by a0(x10). */
    {
    case 1:
        printf("%d", p->R[11]); /* Print a1(x11) as integer. */
        break;

    case 4:
        printf("%s", (char *)&(memory[p->R[11]])); /* Print the string store at address in a11. */ /* may have bugs. */
        break;

    case 10:
        exit(0);
        break;

    case 11:
        printf("%c", p->R[11]); /* Print a1(x11) as a char. */
        break;

    default:                                     /* undefined ecall */
        printf("Illegal ecall number %d\n", -1); /* What stores the ecall arg? */
        exit(-1);
        break;
    }

    p->PC += 4; /* go to the next instruction. */
}

/* Need to be impelmented by Zhangchb. */
void execute_branch(Instruction inst, Processor *p UNUSED)
{
    int offset; /* branch address. */
    offset = 0;
    /* Remember that the immediate portion of branches
        is counting half-words, so make sure to account for that. */
    switch (inst.sbtype.funct3)
    {
    case 0: /* beq */
        offset = (p->R[inst.sbtype.rs1] == p->R[inst.sbtype.rs2]) ? get_branch_offset(inst) : 4;
        break;

    case 1: /* bne */
        offset = (p->R[inst.sbtype.rs1] != p->R[inst.sbtype.rs2]) ? get_branch_offset(inst) : 4;
        break;

    case 4: /* blt */
        offset = (p->R[inst.sbtype.rs1] < p->R[inst.sbtype.rs2]) ? get_branch_offset(inst) : 4;
        break;

    case 5: /* bge */
        offset = (p->R[inst.sbtype.rs1] >= p->R[inst.sbtype.rs2]) ? get_branch_offset(inst) : 4;
        break;

    case 6: /* bltu */
        offset =
            ((unsigned int)p->R[inst.sbtype.rs1] < (unsigned int)p->R[inst.sbtype.rs2]) ? get_branch_offset(inst) : 4;
        break;

    case 7: /* bgeu */
        offset =
            ((unsigned int)p->R[inst.sbtype.rs1] == (unsigned int)p->R[inst.sbtype.rs2]) ? get_branch_offset(inst) : 4;
        break;

    default:
        handle_invalid_instruction(inst);
        exit(-1);
        break;
    }

    p->PC += offset; /* point to the next instruct or jump to the address. */
}

void execute_load(Instruction inst, Processor *p UNUSED, Byte *memory UNUSED)
{
    switch (inst.itype.funct3)
    {
    case 0: /* lb */ /* Check what is check_align later. */
        p->R[inst.itype.rd] = load(memory, p->R[inst.itype.rs1] + get_imm_operand(inst), LENGTH_BYTE, 0);
        break;

    case 1: /* lh */
        p->R[inst.itype.rd] = load(memory, p->R[inst.itype.rs1] + get_imm_operand(inst), LENGTH_HALF_WORD, 0);
        break;

    case 2: /* lw */
        p->R[inst.itype.rd] = load(memory, p->R[inst.itype.rs1] + get_imm_operand(inst), LENGTH_WORD, 0);
        break;

    case 4: /* lbu */ /* check unsigned later. */
        p->R[inst.itype.rd] = (Byte)load(memory, p->R[inst.itype.rs1] + get_imm_operand(inst), LENGTH_BYTE, 0);
        break;

    case 5: /* lhu */
        p->R[inst.itype.rd] = (Half)load(memory, p->R[inst.itype.rs1] + get_imm_operand(inst), LENGTH_HALF_WORD, 0);
        break;

    default:
        handle_invalid_instruction(inst);
        exit(-1);
        break;
    }

    p->PC += 4; /* go to the next instruction. */
}

void execute_store(Instruction inst, Processor *p UNUSED, Byte *memory UNUSED)
{
    switch (inst.stype.funct3)
    {
    case 0: /* sb */
        store(memory, p->R[inst.stype.rs2], LENGTH_BYTE, p->R[inst.stype.rs1], 0);
        break;

    case 1: /* sh */
        store(memory, p->R[inst.stype.rs2], LENGTH_HALF_WORD, p->R[inst.stype.rs1], 0);
        break;

    case 2: /* sw */
        store(memory, p->R[inst.stype.rs2], LENGTH_WORD, p->R[inst.stype.rs1], 0);
        break;

    default:
        handle_invalid_instruction(inst);
        exit(-1);
        break;
    }

    p->PC += 4; /* go to the next instruction. */
}

void execute_jal(Instruction inst UNUSED, Processor *p UNUSED)
{
    Address nextPC;
    nextPC = 0;
    /* Check here later. */
    nextPC = inst.ujtype.imm / 4;
    p->PC = nextPC;
}

void execute_jalr(Instruction inst UNUSED, Processor *p UNUSED)
{
    /* . */
    p->R[inst.itype.rd] = p->PC + 4;
    p->PC = p->R[inst.itype.rs1] + get_imm_operand(inst);
}

void execute_utype(Instruction inst, Processor *p UNUSED)
{
    switch (inst.utype.opcode)
    {
    case 23: /* auipc */ /* Also check PC. */
        p->R[inst.utype.rd] = (p->PC + inst.utype.imm) << 12;
        break;

    case 55: /* lui */
        p->R[inst.utype.rd] = inst.utype.imm << 12;
        break;

    default:
        handle_invalid_instruction(inst);
        exit(-1);
        break;
    }

    p->PC += 4; /* go to the next instruction. */
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
    switch (alignment)
    {
    case LENGTH_BYTE:
        memory[address] = value % 16;
        break;

    case LENGTH_HALF_WORD:
        ((Half *)memory)[address] = value;
        break;

    case LENGTH_WORD:
        ((Word *)memory)[address] = value;
        break;

    default:
        break;
    }
}

Word load(Byte *memory UNUSED, Address address, Alignment alignment, int check_align)
{
    Word data = 0; /* initialize our return value to zero */

    if ((check_align && !check(address, alignment)) || (address >= MEMORY_SPACE))
    {
        handle_invalid_read(address);
    }

    /* YOUR CODE HERE */
    switch (alignment)
    {
    case LENGTH_BYTE:
        data = *((sByte *)(memory + address));
        break;

    case LENGTH_HALF_WORD:
        data = *((sHalf *)(memory + address));
        break;

    case LENGTH_WORD:
        data = *((sWord *)(memory + address));
        break;

    default:
        break;
    }

    return data;
}
