#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "translate_utils.h"

/* This project is based on the MIPS Assembler of CS61C in UC Berkeley.
   The framework of this project is been modified to be suitable for RISC-V
   in CS110 course in ShanghaiTech University by Zhijie Yang in March 2019.
*/

void write_inst_string(FILE* output, const char* name, char** args, int num_args) {
    int i;

    fprintf(output, "%s", name);
    for (i = 0; i < num_args; i++) {
        fprintf(output, " %s", args[i]);
    }
    fprintf(output, "\n");
}

void write_inst_hex(FILE *output, uint32_t instruction) {
    fprintf(output, "%08x\n", instruction);
}

int is_valid_label(const char* str) {
    int first = 1;
    if (!str) {
        return 0;
    }

    while (*str) {
        if (first) {
            if (!isalpha((int) *str) && *str != '_') {
              return 0;   /* does not start with letter or underscore */
            } else {
                first = 0;
            }
        } else if (!isalnum((int) *str) && *str != '_') {
          return 0;       /* subsequent characters not alphanumeric */
        }
        str++;
    }
    return first ? 0 : 1;   /* empty string is invalid  */
}

/* Translate the input string into a signed number. The number is then
   checked to be within the correct range (note bounds are INCLUSIVE)
   ie. NUM is valid if LOWER_BOUND <= NUM <= UPPER_BOUND.

   The input may be in either positive or negative, and be in either
   decimal or hexadecimal format. It is also possible that the input is not
   a valid number. Fortunately, the library function strtol() can take
   care of all that (with a little bit of work from your side of course).
   Please read the documentation for strtol() carefully. Do not use strtoul()
   or any other variants.

   You should store the result into the location that OUTPUT points to. The
   function returns 0 if the conversion proceeded without errors, or -1 if an
   error occurred.
 */
int translate_num(long int* output, const char* str, long int upper_bound,
    long int lower_bound) {

    /* YOUR CODE HERE */
    char* end_ptr;
    long res;

    if (!str || !output) {
        return -1;
    }
    res = strtol(str, &end_ptr, 0);

    if (*end_ptr != '\0') {
        return -1;
    }

    if (res > upper_bound || res < lower_bound) {
        return -1;
    }

    *output = res;

    return 0;

}

int translate_num_bits(long int* output, const char* str, int bits)
{
    char* end_ptr;
    long res;

    /* Obtain the bound of n-bit integer */
    const unsigned long usgn_upper_bound = USNG_UPPER_BOUND(bits);
    const long upper_bound = SNG_UPPER_BOUND(bits);
    const long lower_bound = SNG_LOWER_BOUND(bits);

    if (!str || !output || bits > 64) {
        return -1;
    }

    /* Convert to long integer */
    res = strtol(str, &end_ptr, 0);
    if (*end_ptr != '\0'){
        return -1;
    }

    /* Regarding the return value:
       0 indicates the string is interpreted as decimal
       1 indicates the string is interpreted as hexadecimal
    */
    if(strlen(str) > 2 && str[0] == '0' && str[1] == 'x'){
        /* Hex */
        if ((unsigned long)res > usgn_upper_bound || res < 0) {
            return -1;
        }
        /* Map unsigned hexadecimal to signed interval */
        sign_extension(&res, res, bits);
        *output = res;
        return 1;
    }else{
        /* Decimal */
        /* Check bound */
        if (res > upper_bound || res < lower_bound) {
            return -1;
        }
        *output = res;
        return 0;
    }
}

int sign_extension(long int* output, long input, int bits)
{
    /* check input: the input should never be larger than the unsigned bound */
    /* Regarding return value:
       0 indicates the input is positive, no need to extend
       1 indicates extension happened
       -1 the given unsigned integer falles out of range
       */
    if(0 <= input && (unsigned long)input <= USNG_UPPER_BOUND(bits)){
        if(input > SNG_UPPER_BOUND(bits)){
            /* map unsigned integer to signed integer
               there's a simple law present in the following expression  */
            *output = input - USNG_UPPER_BOUND(bits) - 1;
            return 1;
        }else{
            /* the integer falls on the positive interval */
            *output = input;
            return 0;
        }
    }
    return -1;
}

/*
    Rules for translate 12-bit number: in most cases, 12-bit integer can be
    obtained by 32-bit representation or 12-bit representation. Since some
    instructions just take 32-bit integers, but they are limited with 12 bits,
    32-bit representation is also allowd. For example, 0xFFFFFFFF can be interpreted
    as -1 and allowed to be passed into addi. However, if we only allow 12-bit
    representation, 0xFFFFFFFF are seems too long to be fit in imm field. Hence,
    we should consider both cases.
*/
int translate_num_12(long int* output, const char* str)
{
    long res;
    int ret;
    /* check 12-bit representation first */
	if((ret = translate_num_bits(&res, str, 12)) == -1){
        /* check 32-bit representation then */
        if((ret = translate_num_32(&res, str)) != -1){
            /* remember to check bound when 32-bit representation */
            if(res < SNG_LOWER_BOUND(12) || res > SNG_UPPER_BOUND(12)){
                ret = -1;
            }
        }
    }
    if(ret != -1) *output = res;
    return ret;
}

/*
    Rules for translate 20-bit number are quite similar, but you may note that,
    in most cases, instructions with 20-bit imm field usually take unsigned integers
    as arguments. In those cases, DO NOT USE translate_num_20, for it only interprets
    sign integers.
*/
int translate_num_20(long int* output, const char* str)
{
    long res;
    int ret;
    /* check 20-bit representation first */
	if((ret = translate_num_bits(&res, str, 20)) == -1){
        /* check 32-bit representation then */
        if((ret = translate_num_32(&res, str)) != -1){
            /* remember to check bound when 32-bit representation */
            if(res < SNG_LOWER_BOUND(20) || res > SNG_UPPER_BOUND(20)){
                ret = -1;
            }
        }
    }
    if(ret != -1) *output = res;
    return ret;
}

/* Simply translate from 32-bit representation */
int translate_num_32(long int* output, const char* str)
{
	return translate_num_bits(output, str, 32);
}

/* Translates the register name to the corresponding register number. Please
   see the MIPS Green Sheet for information about register numbers.

   Returns the register number of STR or -1 if the register name is invalid.
 */
int translate_reg(const char* str) {
    /* YOUR CODE HERE */
    if (strcmp(str, "zero") == 0 || strcmp(str, "x0") == 0)      return 0; /* x0 */
    else if (strcmp(str, "ra") == 0 || strcmp(str, "x1") == 0)   return 1; /* x1 */
    else if (strcmp(str, "sp") == 0 || strcmp(str, "x2") == 0)   return 2; /* x2 */
    else if (strcmp(str, "gp") == 0 || strcmp(str, "x3") == 0)   return 3; /* x3 */
    else if (strcmp(str, "tp") == 0 || strcmp(str, "x4") == 0)   return 4; /* x4 */
    else if (strcmp(str, "t0") == 0 || strcmp(str, "x5") == 0)   return 5; /* x5 */
    else if (strcmp(str, "t1") == 0 || strcmp(str, "x6") == 0)   return 6; /* x6 */
    else if (strcmp(str, "t2") == 0 || strcmp(str, "x7") == 0)   return 7; /* x7 */
    else if (strcmp(str, "s0") == 0 || strcmp(str, "x8") == 0 || strcmp(str, "fp") == 0)   return 8; /* x8 */
    else if (strcmp(str, "s1") == 0 || strcmp(str, "x9") == 0)   return 9; /* x9 */
    else if (strcmp(str, "a0") == 0 || strcmp(str, "x10") == 0)   return 10; /* x10 */
    else if (strcmp(str, "a1") == 0 || strcmp(str, "x11") == 0)   return 11; /* x11 */
    else if (strcmp(str, "a2") == 0 || strcmp(str, "x12") == 0)   return 12; /* x12 */
    else if (strcmp(str, "a3") == 0 || strcmp(str, "x13") == 0)   return 13; /* x13 */
    else if (strcmp(str, "a4") == 0 || strcmp(str, "x14") == 0)   return 14; /* x14 */
    else if (strcmp(str, "a5") == 0 || strcmp(str, "x14") == 0)   return 15; /* x15 */
    else if (strcmp(str, "a6") == 0 || strcmp(str, "x16") == 0)   return 16; /* x16 */
    else if (strcmp(str, "a7") == 0 || strcmp(str, "x17") == 0)   return 17; /* x17 */
    else if (strcmp(str, "s2") == 0 || strcmp(str, "x18") == 0)   return 18; /* x18 */
    else if (strcmp(str, "s3") == 0 || strcmp(str, "x19") == 0)   return 19; /* x19 */
    else if (strcmp(str, "s4") == 0 || strcmp(str, "x20") == 0)   return 20; /* x20 */
    else if (strcmp(str, "s5") == 0 || strcmp(str, "x21") == 0)   return 21; /* x21 */
    else if (strcmp(str, "s6") == 0 || strcmp(str, "x22") == 0)   return 22; /* x22 */
    else if (strcmp(str, "s7") == 0 || strcmp(str, "x23") == 0)   return 23; /* x23 */
    else if (strcmp(str, "s8") == 0 || strcmp(str, "x24") == 0)   return 24; /* x24 */
    else if (strcmp(str, "s9") == 0 || strcmp(str, "x25") == 0)   return 25; /* x25 */
    else if (strcmp(str, "s10") == 0 || strcmp(str, "x26") == 0)   return 26; /* x26 */
    else if (strcmp(str, "s11") == 0 || strcmp(str, "x27") == 0)   return 27; /* x27 */
    else if (strcmp(str, "t3") == 0 || strcmp(str, "x28") == 0)   return 28; /* x28 */
    else if (strcmp(str, "t4") == 0 || strcmp(str, "x29") == 0)   return 29; /* x29 */
    else if (strcmp(str, "t5") == 0 || strcmp(str, "x30") == 0)   return 30; /* x30 */
    else if (strcmp(str, "t6") == 0 || strcmp(str, "x31") == 0)   return 31; /* x31 */
    else                                return -1;
}
