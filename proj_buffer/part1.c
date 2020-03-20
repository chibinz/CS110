#include <stdio.h> /* for stderr */
#include <stdlib.h> /* for exit() */
#include "types.h"
#include "utils.h"

void print_rtype(char *, Instruction);
void print_itype_except_load(char *, Instruction);
void print_load(char *, Instruction);
void print_store(char *, Instruction);
void print_branch(char *, Instruction);
void print_utype(char *, Instruction);
void write_rtype(Instruction);
void write_itype_except(Instruction); 
void write_load(Instruction);
void write_store(Instruction);
void write_branch(Instruction);
void write_utype(Instruction);
void write_jal(Instruction);
void write_jalr(Instruction);
void write_ecall(Instruction);


void decode_instruction(Instruction inst) 
{
    switch(inst.opcode.opcode % 128) /* Find the type of the instructions according to its opcode. */
    { 
        case 51:    /* 51 = 0x33 Then it is a Rtype. */
            write_rtype(inst);
            break;

        case 3:     /* 3 = 0x3 Then it is a load instruction in Itype. */
            write_load(inst);
            break;

        case 19:    /* 19 = 0x13 Then it is a Itype. */
            write_itype_except(inst);
            break;

        case 107:   /* 107 = 0x67 Then it is a jalr. */
            write_jalr(inst);
            break;

        case 115:   /* 115 = 0x73 Then it is a ecall */
            write_ecall(inst);
            break;

        case 35:    /* 35 = 0x23 Then it is a store instruction. */
            write_store(inst);
            break;

        case 99:    /* 99 = 0x63 Then it is a SBtype */
            write_branch(inst);
            break;

        case 23:    /* 23 = 0x17 Then it is a utype. */
            write_utype(inst);
            break;

        case 55:    /* 55 = 0x37 Then it is a utype. */
            write_utype(inst);
            break;

        case 111:   /* 111 = 0x6f Then it is a jal. */
            write_jal(inst);
            break;

        default: /* undefined opcode */
            handle_invalid_instruction(inst);
            break;
    }
}

void write_rtype(Instruction inst) 
{
    switch(inst.rtype.funct3) 
    {
        case 0:     /* func3 = 0x0 is add, mul or sub. */
            switch (inst.rtype.funct7)
            {
            case 0: /* func7 = 0x0 is add. */
                print_rtype("add", inst);
                break;

            case 1: /* func7 = 0x1 is mul. */
                print_rtype("mul", inst);
                break;
            
            case 32:/* func7 = 0x20 is sub. */
                print_rtype("sub", inst);
                break;

            default:
                handle_invalid_instruction(inst);
                break;
            }
            break;

        case 1:
            switch (inst.rtype.funct7)
            {
            case 0: /* func7 = 0x0 is sll. */
                print_rtype("sll", inst);
                break;
            
            case 1: /* func7 = 1 is mulh. */
                print_rtype("mulh", inst);
                break;

            default:
                handle_invalid_instruction(inst);
                break;
            }
            break;

        case 2:     /* func3 = 0x2 is slt. */
            print_rtype("slt", inst);
            break;

        case 3:     /* func3 = 0x3 is slut. */
            print_rtype("slut", inst);
            break;

        case 4:     
            switch (inst.rtype.funct7)
            {
            case 0: /* func7 = 0x0 is xor. */
                print_rtype("xor", inst);

                break;
            
            case 1: /* func7 = 0x1 is div. */
                print_rtype("div", inst);
                break;

            default:
                handle_invalid_instruction(inst);
                break;
            }
            break;

        case 5:
            switch (inst.rtype.funct7)
            {
            case 0: /* func7 = 0x0 is srl. */
                print_rtype("srl", inst);
                break;
            
            case 32:/* func7 = 0x20 is sra. */
                print_rtype("sra", inst);
                break;

            default:
                handle_invalid_instruction(inst);
                break;
            }
            break;

        case 6:
            switch (inst.rtype.funct7)
            {
            case 0: /* func7 = 0x0 is or. */
                print_rtype("or", inst);
                break;
            
            case 1: /* func7 = 0x1 is rem. */
                print_rtype("rem", inst);
                break;

            default:
                handle_invalid_instruction(inst);
                break;
            }
            break;

        case 7: /* func3 = 0x7 is and. */
            print_rtype("and", inst);
            break;

        default:
            handle_invalid_instruction(inst);
            break;
    }
}

void write_itype_except(Instruction inst) 
{
    int shiftOp;
    shiftOp = -1;

    switch(inst.itype.funct3) 
    { 
    case 0:     /* func3 = 0x0 is addi */
        print_itype_except_load("addi", inst);
        break;

    case 1:     /* func3 = 0x1 is slli. */
        print_itype_except_load("slli", inst);
        break;

    case 2:     /* func3 = 0x2 is slti. */
        print_itype_except_load("slti", inst);
        break;

    case 3:     /* ........... is sltiu. */
        print_itype_except_load("sltiu", inst);
        break;

    case 4:     /* ... is xori.*/
        print_itype_except_load("xori", inst);
        break;
    
    case 5:     
        switch (inst.rtype.funct7)
        {
        case 0: /* ... is srli. */
            print_itype_except_load("srli", inst);
            break;
        
        case 32:/* ... is srai. */
            print_itype_except_load("srai", inst);
            break;

        default:
            handle_invalid_instruction(inst);
            break;
        }
        break;
    
    case 6:     /* .. is ori. */
        print_itype_except_load("ori", inst);
        break;
    
    case 7:     /* ... is andi. */
        print_itype_except_load("andi", inst);
        break;

    default:
        handle_invalid_instruction(inst);
        break;  
    }
}

void write_load(Instruction inst) 
{
    switch(inst.itype.funct3) 
    { 
        case 0:     /* .. is lb. */
            print_load("lb", inst);
            break;

        case 1:     /* .. is lh. */
            print_load("lh", inst);
            break;

        case 2:     /* .. is lw. */
            print_load("lw", inst);
            break;

        case 4:     /* .. is lbu. */
            print_load("lbu", inst);
            break;
        
        case 5:     /* .. is lhu. */
            print_load("lhu", inst);
            break;

        default:
            handle_invalid_instruction(inst);
            break;
    }
}

void write_store(Instruction inst) 
{
    switch(inst.stype.funct3) 
    { 
        case 0:     /* .. is sb. */
            print_store("sb", inst);
            break;

        case 1:     /* .. is sh. */
            print_store("sh", inst);
            break;

        case 2:     /* .. is sw. */
            print_store("sw", inst);
            break;

        default:
            handle_invalid_instruction(inst);
            break;
    }
}

void write_branch(Instruction inst) 
{
    switch(inst.sbtype.funct3) 
    {
        case 0:     /* .. is beq. */
            print_branch("beq", inst);
            break;

        case 1:     /* .. is bne. */
            print_branch("bne", inst);
            break;

        case 4:     /* .. is blt. */
            print_branch("blt", inst);
            break;

        case 5:     /* .. si bge. */
            print_branch("bge", inst);
            break;
        
        case 6:     /* .. is bltu. */
            print_branch("bltu", inst);
            break;

        case 7:     /* .. is begu. */
            print_branch("bgeu", inst);
            break;

        default:
            handle_invalid_instruction(inst);
            break;
    }
}

/* For the writes, probably a good idea to take a look at utils.h */

void write_utype(Instruction inst) 
{
    switch(inst.utype.opcode) 
    { 
        case 23:    /* .. is auipc. */
            print_utype("auipc", inst);
            break;
            
        case 55:    /* .. is lui. */
            print_utype("lui", inst);
            break;

        default:
            handle_invalid_instruction(inst);
            break;
    }

}

void write_jal(Instruction inst UNUSED) 
{
    printf(JAL_FORMAT, inst.ujtype.rd, get_jump_offset(inst));
}

void write_jalr(Instruction inst UNUSED) 
{
    printf(ITYPE_FORMAT, "jalr", inst.itype.rd, inst.itype.rs1, get_imm_operand(inst));
}

void write_ecall(Instruction inst UNUSED) 
{
    /* Maybe here needs to check whether func3 or func7 is 0. */
    printf(ECALL_FORMAT);
}

/* Print: given an instruction and its name, print it. */
void print_rtype(char *name UNUSED, Instruction inst UNUSED) 
{
    printf(RTYPE_FORMAT, name, inst.rtype.rd, inst.rtype.rs1, inst.rtype.rs2);
}

void print_itype_except_load(char *name UNUSED, Instruction inst UNUSED) 
{
    printf(ITYPE_FORMAT, name, inst.itype.rd, inst.itype.rs1, get_imm_operand(inst));
}

void print_load(char *name UNUSED, Instruction inst UNUSED) 
{
    printf(MEM_FORMAT, name, inst.itype.rd, get_imm_operand(inst), inst.itype.rs1);
}

void print_store(char *name UNUSED, Instruction instruction UNUSED) 
{
    printf(MEM_FORMAT, name, instruction.stype.rs2, get_store_offset(instruction), instruction.stype.rs1);
}

void print_branch(char *name UNUSED, Instruction inst UNUSED) 
{
    printf(BRANCH_FORMAT, name, inst.sbtype.rs1, inst.sbtype.rs2, get_branch_offset(inst));
}

void print_utype(char *name UNUSED, Instruction inst UNUSED) 
{
    printf(UTYPE_FORMAT, name, inst.utype.rd, inst.utype.imm);
}
