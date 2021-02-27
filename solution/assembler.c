#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "src/tables.h"
#include "src/translate.h"
#include "src/translate_utils.h"
#include "src/utils.h"

#include "assembler.h"

const int MAX_ARGS = 3;
const int BUF_SIZE = 1024;
const char *IGNORE_CHARS = " \f\n\r\t\v,()";

/*******************************
 * Helper Functions
 *******************************/

/* you should not be calling this function yourself. */
static void raise_label_error(uint32_t input_line, const char *label) {
    write_to_log("Error - invalid label at line %d: %s\n", input_line, label);
}

/* call this function if more than MAX_ARGS arguments are found while parsing
   arguments.

   INPUT_LINE is which line of the input file that the error occurred in. Note
   that the first line is line 1 and that empty lines are included in the count.

   EXTRA_ARG should contain the first extra argument encountered.
 */
static void raise_extra_argument_error(uint32_t input_line,
                                       const char *extra_arg) {
    write_to_log("Error - extra argument at line %d: %s\n", input_line,
                 extra_arg);
}

/* You should call this function if write_pass_one() or translate_inst()
   returns -1.

   INPUT_LINE is which line of the input file that the error occurred in. Note
   that the first line is line 1 and that empty lines are included in the count.
 */
static void raise_instruction_error(uint32_t input_line, const char *name,
                                    char **args, int num_args) {

    write_to_log("Error - invalid instruction at line %d: ", input_line);
    log_inst(name, args, num_args);
}

/* Truncates the string at the first occurrence of the '#' character. */
static void skip_comments(char *str) {
    char *comment_start = strchr(str, '#');
    if (comment_start) {
        *comment_start = '\0';
    }
}

/* Reads STR and determines whether it is a label (ends in ':'), and if so,
   whether it is a valid label, and then tries to add it to the symbol table.

   INPUT_LINE is which line of the input file we are currently processing. Note
   that the first line is line 1 and that empty lines are included in this
   count.

   BYTE_OFFSET is the offset of the NEXT instruction (should it exist).

   Four scenarios can happen:
    1. STR is not a label (does not end in ':'). Returns 0.
    2. STR ends in ':', but is not a valid label. Returns -1.
    3a. STR ends in ':' and is a valid label. Addition to symbol table fails.
        Returns -1.
    3b. STR ends in ':' and is a valid label. Addition to symbol table succeeds.
        Returns 1.
 */
static int add_if_label(uint32_t input_line, char *str, uint32_t byte_offset,
                        SymbolTable *symtbl) {

    size_t len = strlen(str);
    if (str[len - 1] == ':') {
        str[len - 1] = '\0';
        if (is_valid_label(str)) {
            if (add_to_table(symtbl, str, byte_offset) == 0) {
                return 1;
            } else {
                return -1;
            }
        } else {
            raise_label_error(input_line, str);
            return -1;
        }
    } else {
        return 0;
    }
}

/*******************************
 * Implement the Following
 *******************************/

/* First pass of the assembler. You should implement pass_two() first.

   This function should read each line, strip all comments, scan for labels,
   and pass instructions to write_pass_one(). The input file may or may not
   be valid. Here are some guidelines:

    1. Only one label may be present per line. It must be the first token
   present. Once you see a label, regardless of whether it is a valid label or
   invalid label, treat the NEXT token as the beginning of an instruction.
    2. If the first token is not a label, treat it as the name of an
   instruction.
    3. Everything after the instruction name should be treated as arguments to
        that instruction. If there are more than MAX_ARGS arguments, call
        raise_extra_argument_error() and pass in the first extra argument. Do
   not write that instruction to the output file (eg. don't call
   write_pass_one())
    4. Only one instruction should be present per line. You do not need to do
        anything extra to detect this - it should be handled by guideline 3.
    5. A line containing only a label is valid. The address of the label should
        be the byte offset of the next instruction, regardless of whether there
        is a next instruction or not.

   Just like in pass_two(), if the function encounters an error it should NOT
   exit, but process the entire file and return -1. If no errors were
   encountered, it should return 0.
 */
int pass_one(FILE *output, FILE *input, SymbolTable *symtbl) {
    /* YOUR CODE HERE */

    /* a buffer for line parse */
    char buf[BUF_SIZE];

    char *args[3], name[20], *next; /*Declares the chars*/
    int error = 0, label = 0, i = 0, pass = 0, result = 0,
        flag = 0;      /*Declares the ints*/
    int lineCount = 0; /*Declares the unsigned ints*/
    uint32_t byteOffset = 0;

    while (fgets(buf, BUF_SIZE, input)) /*Reads each line*/
    {
        pass = 1;
        /* skip all the comments */
        skip_comments(buf);               /*Removes the comments*/
        next = strtok(buf, IGNORE_CHARS); /*Gets the next token*/
        lineCount++;
        if (next != NULL) /*If next is not NULL*/
        {
            error = add_if_label(lineCount, next, byteOffset,
                                 symtbl); /*Defines error*/
            if (error != 0) {
                next =
                    strtok(NULL, IGNORE_CHARS); /*If error != 0, update next*/
            }
            if (next != NULL) /*If next is not NULL*/
            {
                strcpy(name, next); /*Copies next into name*/
                i = 0;
                next = strtok(NULL, IGNORE_CHARS); /*Updates next*/
                while (next != NULL) {
                    args[i] = next; /*Replaces the ith slice with next*/
                    i++;
                    if (i > MAX_ARGS) {
                        pass = 0;
                        raise_extra_argument_error(
                            lineCount,
                            next); /*Raises such error if i exceeds max args*/
                        /* error = -1;  */ /*Updates the error by -1*/
                        flag = 1;
                        break; /*Breaks the iteration*/
                    }
                    next = strtok(NULL, IGNORE_CHARS); /*Updates next*/
                }
                if (flag) {
                    continue;
                }
                label = write_pass_one(output, name, args,
                                       i); /*Get written line count*/
                if (label == 0 && pass) {
                    raise_instruction_error(
                        lineCount, name, args,
                        i);     /*Raise instruction error if label == 0*/
                    error = -1; /*Updates error by -1*/
                }
                byteOffset += 4 * label; /*Increase instruction memory byte
                                            offset by 4 * label*/
            }
            if (error == -1 || flag == 1) {
                result = -1;
            }
        }
    }

    return result; /*Returns*/
}

/* Reads an intermediate file and translates it into machine code. You may
   assume:
    1. The input file contains no comments
    2. The input file contains no labels
    3. The input file contains at maximum one instruction per line
    4. All instructions have at maximum MAX_ARGS arguments
    5. The symbol table has been filled out already
   If an error is reached, DO NOT EXIT the function. Keep translating the rest
   of the document, and at the end, return -1. Return 0 if no errors were
   encountered. */
int pass_two(FILE *output, FILE *input, SymbolTable *symtbl,
             SymbolTable *reltbl) {
    /* YOUR CODE HERE */

    /* Since we pass this buffer to strtok(),
       the chars here will GET CLOBBERED.  */
    char buf[BUF_SIZE];

    /* Store input line number / byte offset below.
       When should each be incremented?  */

    /* First, read the next line into a buffer. */
    char *args[4], *next;
    int i = 0, error = 0;
    unsigned lineCount = 0;
    uint32_t byteOffset = 0;
    /*Declarations before any control expressions*/
    while (fgets(buf, BUF_SIZE, input)) /*Reads every line*/
    {
        char *args_name = strtok(buf, IGNORE_CHARS);
        if (!args_name) /*If args_name is not empty*/
            error = -1; /*Sets error to -1*/
        lineCount++;    /*Increase line counter*/
        i = 0;
        while ((next = strtok(NULL, IGNORE_CHARS))) {
            if (i >= MAX_ARGS) {
                raise_extra_argument_error(
                    lineCount,
                    next);  /*Raises such error if i exceeds max args*/
                error = -1; /*Sets error to -1*/
            } else {
                args[i] = next; /*Updates args with next*/
                i++;
            }
        }
        if (translate_inst(output, args_name, args, i, byteOffset, symtbl,
                           reltbl) == -1) {
            raise_instruction_error(lineCount, args_name, args,
                                    i); /*If translation fails*/
            error = -1;                 /*Sets error to -1*/
        }
        if (error == 0)      /*If no error*/
            byteOffset += 4; /*Update byte offset*/
    }
    return error; /*Return error finally*/
}

/*******************************
 * Do Not Modify Code Below
 *******************************/

static int open_files(FILE **input, FILE **output, const char *input_name,
                      const char *output_name) {

    *input = fopen(input_name, "r");
    if (!*input) {
        write_to_log("Error: unable to open input file: %s\n", input_name);
        return -1;
    }
    *output = fopen(output_name, "w");
    if (!*output) {
        write_to_log("Error: unable to open output file: %s\n", output_name);
        fclose(*input);
        return -1;
    }
    return 0;
}

static void close_files(FILE *input, FILE *output) {
    fclose(input);
    fclose(output);
}

/* Runs the two-pass assembler. Most of the actual work is done in pass_one()
   and pass_two().
 */
int assemble(const char *out, const char *in, const char *tmp) {
    FILE *src, *dst;
    int err = 0;
    SymbolTable *symtbl = create_table(SYMBOLTBL_UNIQUE_NAME);
    SymbolTable *reltbl = create_table(SYMBOLTBL_NON_UNIQUE);

    if (in) {
        printf("Running pass one: %s -> %s\n", in, tmp);
        if (open_files(&src, &dst, in, tmp) != 0) {
            free_table(symtbl);
            free_table(reltbl);
            exit(1);
        }

        if (pass_one(dst, src, symtbl) != 0) {
            err = 1;
        }
        close_files(src, dst);
    }

    if (out) {
        printf("Running pass two: %s -> %s\n", tmp, out);
        if (open_files(&src, &dst, tmp, out) != 0) {
            free_table(symtbl);
            free_table(reltbl);
            exit(1);
        }

        fprintf(dst, ".text\n");
        if (pass_two(dst, src, symtbl, reltbl) != 0) {
            err = 1;
        }

        fprintf(dst, "\n.symbol\n");
        write_table(symtbl, dst);

        fprintf(dst, "\n.relocation\n");
        write_table(reltbl, dst);

        close_files(src, dst);
    }

    free_table(symtbl);
    free_table(reltbl);
    return err;
}

static void print_usage_and_exit() {
    printf("Usage:\n");
    printf("  Runs both passes: assembler <input file> <intermediate file> "
           "<output file>\n");
    printf(
        "  Run pass #1:      assembler -p1 <input file> <intermediate file>\n");
    printf("  Run pass #2:      assembler -p2 <intermediate file> <output "
           "file>\n");
    printf("Append -log <file name> after any option to save log files to a "
           "text file.\n");
    exit(0);
}

int main(int argc, char **argv) {
    if (argc != 4 && argc != 6) {
        print_usage_and_exit();
    }

    int mode = 0;
    if (strcmp(argv[1], "-p1") == 0) {
        mode = 1;
    } else if (strcmp(argv[1], "-p2") == 0) {
        mode = 2;
    }

    char *input, *inter, *output;
    if (mode == 1) {
        input = argv[2];
        inter = argv[3];
        output = NULL;
    } else if (mode == 2) {
        input = NULL;
        inter = argv[2];
        output = argv[3];
    } else {
        input = argv[1];
        inter = argv[2];
        output = argv[3];
    }

    if (argc == 6) {
        if (strcmp(argv[4], "-log") == 0) {
            set_log_file(argv[5]);
        } else {
            print_usage_and_exit();
        }
    }

    int err = assemble(output, input, inter);

    if (err) {
        write_to_log(
            "One or more errors encountered during assembly operation.\n");
    } else {
        write_to_log("Assembly operation completed successfully!\n");
    }

    if (is_log_file_set()) {
        printf("Results saved to %s\n", argv[5]);
    }

    return err;
}
