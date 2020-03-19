#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

/* You may find implementing this function helpful */
int bitSigner(unsigned int field UNUSED, unsigned int size UNUSED)
{
    /* YOUR CODE HERE */
    return 0;
}

/* You may find implementing this function handy */
int get_imm_operand(Instruction inst UNUSED)
{
    /* YOUR CODE HERE */
    return 0;
}

/* Remember that the offsets should return the offset in BYTES */

int get_branch_offset(Instruction inst UNUSED)
{
    /* YOUR CODE HERE */
    return 0;
}

int get_jump_offset(Instruction inst UNUSED)
{
    /* YOUR CODE HERE */
    return 0;
}

int get_store_offset(Instruction inst UNUSED)
{
    /* YOUR CODE HERE */
    return 0;
}

<<<<<<< HEAD
int get_u_offset(Instruction inst)
{

    return 0;
}

void handle_invalid_instruction(Instruction inst)
{
    printf("Invalid Instruction: 0x%08x\n", inst.bits);
}

=======
void handle_invalid_instruction(Instruction inst)
{
    printf("Invalid Instruction: 0x%08x\n", inst.bits);
}

>>>>>>> 94052eb8325cbfc9c325d83c5c023b182b18d1e5
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
