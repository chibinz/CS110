#ifndef TRANSLATE_UTILS_H
#define TRANSLATE_UTILS_H

#include <stdint.h>

/** upper bound of int64 */
#define _INT64_MAX_ (long)0x7FFFFFFFFFFFFFFF
/** lower bound of int64 */
#define _INT64_MIN_ (long)0x8000000000000000
/** upper bound of unsigned int64 */
#define _UINT64_MAX_ (unsigned long)0xFFFFFFFFFFFFFFFF

/** upper bound of int32 */
#define _INT32_MAX_ (long)0x000000007FFFFFFF
/** lower bound of int32 */
#define _INT32_MIN_ (long)0xFFFFFFFF80000000
/** upper bound of unsigned int32 */
#define _UINT32_MAX_ (unsigned long)0x00000000FFFFFFFF

/** upper bound of int12 */
#define _INT12_MAX_ (long)0x00000000000007FF
/** lower bound of int12 */
#define _INT12_MIN_ (long)0xFFFFFFFFFFFFF800
/** upper bound of unsigned int12 */
#define _UINT12_MAX_ (unsigned long)0x0000000000000FFF

/** upper bound of int20 */
#define _INT20_MAX_ (long)0x000000000007FFFF
/** lower bound of int20 */
#define _INT20_MIN_ (long)0xFFFFFFFFFFF80000
/** upper bound of unsigned int20 */
#define _UINT20_MAX_ (unsigned long)0x00000000000FFFFF

/* Unsigned upper bound: 2^k - 1 */
#define USNG_UPPER_BOUND(bits) (unsigned long)((_UINT64_MAX_ << (64 - bits)) >> (64 - bits))
/* Signed upper bound: 2^(k-1) - 1 */
#define SNG_UPPER_BOUND(bits) (long)((_UINT64_MAX_ << (64 - bits + 1)) >> (64 - bits + 1))
/* Signed lower bound: -2^(k-1) */
#define SNG_LOWER_BOUND(bits) (long)((_UINT64_MAX_ >> (bits - 1)) << (bits - 1))

/* Writes the instruction as a string to OUTPUT. NAME is the name of the
   instruction, and its arguments are in ARGS. NUM_ARGS is the length of
   the array.
 */
void write_inst_string(FILE* output, const char* name, char** args, int num_args);

/* Writes the instruction to OUTPUT in hexadecimal format. */
void write_inst_hex(FILE* output, uint32_t instruction);

/* Returns 1 if the label is valid and 0 if it is invalid. A valid label is one
   where the first character is a character or underscore and the remaining
   characters are either characters, digits, or underscores.
 */
int is_valid_label(const char* str);



/* IMPLEMENT ME - see documentation in translate_utils.c */
int translate_num(long int* output, const char* str, long int lower_bound,
	long int upper_bound);

/* Note that the following function convert string to signed int
   thus, you may not use it in instructions which operate on unsigned operand
*/
int translate_num_bits(long int* output, const char* str, int bits);

/* Sign extension: regard the bits-th bit as the most significant bit, and
   set it to all forwarding bits. This kind of extension can convert small-length
   words into 64-bits integer without changing the origin value
*/
int sign_extension(long int* output, long input, int bits);

/* The following methods are handy utilities for signed number translation */
int translate_num_12(long int* output, const char* str);
int translate_num_20(long int* output, const char* str);
int translate_num_32(long int* output, const char* str);

/* IMPLEMENT ME - see documentation in translate_utils.c */
int translate_reg(const char* str);

#endif
