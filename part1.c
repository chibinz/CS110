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
    unsigned int func3, func7;
    func3 = (inst.opcode.opcode >> 12) % 8;
    func7 = (inst.opcode.opcode >> 25) % 128;
    
    switch(func3) 
    {
        case 0:     /* func3 = 0x0 is add, mul or sub. */
            switch (func7)
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

        case 1:
            switch (func7)
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

        case 2:     /* func3 = 0x2 is slt. */
            print_rtype("slt", inst);
            break;

        case 3:     /* func3 = 0x3 is slut. */
            print_rtype("slut", inst);
            break;

        case 4:     
            switch (func7)
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

        case 5:
            switch (func7)
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

        case 6:
            switch (func7)
            {
            case 0: /* func7 = 0x0 is or. */
                print_rtype("or", inst);
            
            case 1: /* func7 = 0x1 is rem. */
                print_rtype("rem", inst);

            default:
                handle_invalid_instruction(inst);
                break;
            }

        case 7: /* func3 = 0x7 is and. */
            print_rtype("and", inst);

        default:
            handle_invalid_instruction(inst);
            break;
    }
}

void write_itype_except(Instruction inst) 
{
    int shiftOp;
    unsigned int func3, func7;
    func3 = (inst.opcode.opcode >> 12) % 8;
    func7 = (inst.opcode.opcode >> 25) % 128;
    shiftOp = -1;

    switch(func3) 
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
        switch (func7)
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
    unsigned int func3;
    func3 = (inst.opcode.opcode >> 12) % 8;
    switch(func3) 
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
    switch(0) 
    { 

        default:
            handle_invalid_instruction(inst);
            break;
    }
}

void write_branch(Instruction inst) 
{
    switch(0) 
    { /* What do we switch on? */
        /* YOUR CODE HERE */

        default:
            handle_invalid_instruction(inst);
            break;
    }
}

/* For the writes, probably a good idea to take a look at utils.h */

void write_utype(Instruction inst) 
{
    switch(0) 
    { /* What do we switch on? */
        /* YOUR CODE HERE */

        default:
            handle_invalid_instruction(inst);
            break;
    }

}

void write_jal(Instruction inst UNUSED) 
{
    /* YOUR CODE HERE */

}

void write_jalr(Instruction inst UNUSED) 
{
    /* YOUR CODE HERE */

}

void write_ecall(Instruction inst UNUSED) 
{
    /* YOUR CODE HERE */

}

/* Print: given an instruction, return its name in name. (zhangchb may do this) */
void print_rtype(char *name UNUSED, Instruction inst UNUSED) 
{
    /* YOUR CODE HERE */

}

void print_itype_except_load(char *name UNUSED, Instruction inst UNUSED) 
{
    /* YOUR CODE HERE */

}

void print_load(char *name UNUSED, Instruction inst UNUSED) 
{
    /* YOUR CODE HERE */

}

void print_store(char *name UNUSED, Instruction instruction UNUSED) 
{
    /* YOUR CODE HERE */

}

void print_branch(char *name UNUSED, Instruction inst UNUSED) 
{
    /* YOUR CODE HERE */

}

void print_utype(char *name UNUSED, Instruction inst UNUSED) 
{
    /* YOUR CODE HERE */

}
