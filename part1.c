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
    /* YOUR CODE HERE: COMPLETE THE SWITCH STATEMENTS */
    switch(0) { /* What do we switch on? */
        /* YOUR CODE HERE */

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
