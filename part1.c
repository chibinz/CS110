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


void decode_instruction(Instruction inst) {

    switch(inst.opcode.opcode % 128) { /* What do we switch on? */

        case 51:    /* 51 = 0x33 Then it is a Rtype. */
            write_rtype(inst);

        case 3:     /* 3 = 0x3 Then it is a load instruction in Itype. */
            write_load(inst);

        case 19:    /* 19 = 0x13 Then it is a Itype. */
            write_itype_except(inst);

        case 107:   /* 107 = 0x67 Then it is a jalr. */
            write_jalr(inst);

        case 115:   /* 115 = 0x73 Then it is a ecall */
            write_ecall(inst);

        case 35:    /* 35 = 0x23 Then it is a store instruction. */
            write_store(inst);

        case 99:    /* 99 = 0x63 Then it is a SBtype */
            write_branch(inst);

        case 23:    /* 23 = 0x17 Then it is a utype. */
            write_utype(inst);

        case 55:    /* 55 = 0x37 Then it is a utype. */
            write_utype(inst);

        case 111:   /* 111 = 0x6f Then it is a jal. */
            write_jal(inst);

        default: /* undefined opcode */
            handle_invalid_instruction(inst);
            break;
    }
}

void write_rtype(Instruction inst) {
    switch(0) { /* What do we switch on? */
        /* YOUR CODE HERE */

        default:
            handle_invalid_instruction(inst);
            break;
    }
}

void write_itype_except(Instruction inst) {
    int shiftOp;
    shiftOp = -1;
    switch(0) { /* What do we switch on? */
        /* YOUR CODE HERE */

        default:
            handle_invalid_instruction(inst);
            break;  
    }
}

void write_load(Instruction inst) {
    switch(0) { /* What do we switch on? */
        /* YOUR CODE HERE */

        default:
            handle_invalid_instruction(inst);
            break;
    }
}

void write_store(Instruction inst) {
    switch(0) { /* What do we switch on? */
        /* YOUR CODE HERE */

        default:
            handle_invalid_instruction(inst);
            break;
    }
}

void write_branch(Instruction inst) {
    switch(0) { /* What do we switch on? */
        /* YOUR CODE HERE */

        default:
            handle_invalid_instruction(inst);
            break;
    }
}

/* For the writes, probably a good idea to take a look at utils.h */

void write_utype(Instruction inst) {
    switch(0) { /* What do we switch on? */
        /* YOUR CODE HERE */

        default:
            handle_invalid_instruction(inst);
            break;
    }

}

void write_jal(Instruction inst UNUSED) {
    /* YOUR CODE HERE */

}

void write_jalr(Instruction inst UNUSED) {
    /* YOUR CODE HERE */

}

void write_ecall(Instruction inst UNUSED) {
    /* YOUR CODE HERE */

}

void print_rtype(char *name UNUSED, Instruction inst UNUSED) {
    /* YOUR CODE HERE */

}

void print_itype_except_load(char *name UNUSED, Instruction inst UNUSED) {
    /* YOUR CODE HERE */

}

void print_load(char *name UNUSED, Instruction inst UNUSED) {
    /* YOUR CODE HERE */

}

void print_store(char *name UNUSED, Instruction instruction UNUSED) {
    /* YOUR CODE HERE */

}

void print_branch(char *name UNUSED, Instruction inst UNUSED) {
    /* YOUR CODE HERE */

}

void print_utype(char *name UNUSED, Instruction inst UNUSED) {
    /* YOUR CODE HERE */

}
