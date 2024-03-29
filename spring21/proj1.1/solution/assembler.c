/* This project is based on the MIPS Assembler of CS61C in UC Berkeley.
   The framework of this project is been modified to be suitable for RISC-V
   in CS110 course in ShanghaiTech University by Zhijie Yang in March 2019.
   Updated by Chibin Zhang and Zhe Ye in March 2021.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "src/utils.h"
#include "src/tables.h"
#include "src/translate_utils.h"
#include "src/translate.h"

#include "assembler.h"

const int MAX_ARGS = 3;
const int BUF_SIZE = 1024;
const char* IGNORE_CHARS = " \f\n\r\t\v,()";

/*******************************
 * Helper Functions
 *******************************/

/* you should not be calling this function yourself. */
static void raise_label_error(uint32_t input_line, const char* label) {
    write_to_log("Error - invalid label at line %d: %s\n", input_line, label);
}

/* call this function if more than MAX_ARGS arguments are found while parsing
   arguments.

   INPUT_LINE is which line of the input file that the error occurred in. Note
   that the first line is line 1 and that empty lines are included in the count.

   EXTRA_ARG should contain the first extra argument encountered.
 */
static void raise_extra_argument_error(uint32_t input_line, const char* extra_arg) {
    write_to_log("Error - extra argument at line %d: %s\n", input_line, extra_arg);
}

/* You should call this function if write_pass_one() or translate_inst()
   returns -1.

   INPUT_LINE is which line of the input file that the error occurred in. Note
   that the first line is line 1 and that empty lines are included in the count.
 */
static void raise_instruction_error(uint32_t input_line, const char* name, char** args,
    int num_args) {

    write_to_log("Error - invalid instruction at line %d: ", input_line);
    log_inst(name, args, num_args);
}

/* Truncates the string at the first occurrence of the '#' character. */
static void skip_comments(char* str) {
    char* comment_start = strchr(str, '#');
    if (comment_start) {
        *comment_start = '\0';
    }
}

/* Reads STR and determines whether it is a label (ends in ':'), and if so,
   whether it is a valid label, and then tries to add it to the symbol table.

   INPUT_LINE is which line of the input file we are currently processing. Note
   that the first line is line 1 and that empty lines are included in this count.

   BYTE_OFFSET is the offset of the NEXT instruction (should it exist).

   Four scenarios can happen:
    1. STR is not a label (does not end in ':'). Returns 0.
    2. STR ends in ':', but is not a valid label. Returns -1.
    3a. STR ends in ':' and is a valid label. Addition to symbol table fails.
        Returns -1.
    3b. STR ends in ':' and is a valid label. Addition to symbol table succeeds.
        Returns 1.
 */
static int add_if_label(uint32_t input_line, char* str, uint32_t byte_offset,
    SymbolTable* symtbl) {

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
   and pass instructions to write_pass_one(). The symbol table should also
   been built and written to specified file. The input file may or may not
   be valid. Here are some guidelines:

    1. Only one label may be present per line. It must be the first token present.
        Once you see a label, regardless of whether it is a valid label or invalid
        label, treat the NEXT token as the beginning of an instruction.
    2. If the first token is not a label, treat it as the name of an instruction.
    3. Everything after the instruction name should be treated as arguments to
        that instruction. If there are more than MAX_ARGS arguments, call
        raise_extra_argument_error() and pass in the first extra argument. Do not
        write that instruction to the output file (eg. don't call write_pass_one())
    4. Only one instruction should be present per line. You do not need to do
        anything extra to detect this - it should be handled by guideline 3.
    5. A line containing only a label is valid. The address of the label should
        be the byte offset of the next instruction, regardless of whether there
        is a next instruction or not.

   Just like in pass_two(), if the function encounters an error it should NOT
   exit, but process the entire file and return -1. If no errors were encountered,
   it should return 0.
 */
int pass_one(FILE* input, FILE* inter, FILE* symtbl) {
    /* YOUR CODE HERE */

    SymbolTable *sym = create_table(SYMBOLTBL_UNIQUE_NAME);

    /* Counter of errors */
    int errors = 0;

    /* a buffer for line parse */
    char buf[BUF_SIZE];

    /* store input line number / byte offset below */
    uint32_t lineNumber = 0, byteOffset = 0;

    /* First, read next line into buffer */
    char *args[MAX_ARGS];
    int num_args;

    /* Define delimiters. As IGNORE_CHARS has already been defined */
    const char* delimiters = IGNORE_CHARS;
    char *name = NULL, *token = NULL;

    int insWritten = 0;

    /* For each line, the following issues are regarded as errors
       1. Invalid label name / or failure on label insertion
       2. Extra arguments
       */
    while(fgets(buf, sizeof(buf), input) != NULL){

        /* First accumulate line number */
        lineNumber++;

        /* skip all the comments */
        skip_comments(buf);
        /* Ignore the empty lines */
        if(strlen(buf) == 0) continue;

        /* use strtok() to read next token*/
        token = strtok(buf, delimiters);
        /* Ignore the empty first token(label or instrution name) */
        if(token == NULL) continue;

        /* deal with label */
        switch(add_if_label(lineNumber, token, byteOffset, sym)){
            case 0:
                /* Here the first token is just regared as the instrution name */
                break;
            case 1:
                /* Glob the next token as the instrution name
                   when the first token is classified to be label
                   */
                token = strtok(NULL, delimiters);
                break;
            case -1:
                /* Error occurs, skip this line */
                errors++;
                continue;
        }

        /* ignore the empty instrution name.
           this might be the token read skipping the label
           or exactly the first incoming token
           */
        if(token == NULL) continue;

        /* assign the instruction name */
        name = token;

        /* parse the instrution */
        num_args = 0;
        while((token = strtok(NULL, delimiters)) != NULL){
            /* accumulate num_args
               note that after num_args accumulated, any checkup
               or operations on current argument should consider substract
               one from the current num_args */
            num_args++;

            /* check upper bound of arguments */
            if(num_args > MAX_ARGS){
                raise_extra_argument_error(lineNumber, token);
                break;
            }

            args[num_args - 1] = token;
        }

        /* Proceed only if argument are correct */
        if(num_args > MAX_ARGS){
            errors++;
            continue;
        }

        insWritten = write_pass_one(inter, name, args, num_args);
        if(insWritten == 0){
            /* If an error occurs,
               the instruction will not be written and you should call
               raise_instruction_error().  */
            raise_instruction_error(lineNumber, name, args, num_args);
            errors++;
            continue;
        }

        /* 1. accumulate byte offset
           2. byte offset is only accumulated when the current line
           contains a valid instruction. ALL THINGS SHOULD BE FINE HERE.
           3. 32-bit system, 4 bytes for a single instruction
           */
        byteOffset += insWritten * 4;
    }

    write_table(sym, symtbl);
    free_table(sym);

    return errors > 0 ? -1 : 0;
}

/* Second pass of the assembler.

   This function should read an intermediate file and the corresponding symbol table
   file, translates it into machine code. You may assume:
    1. The input file contains no comments
    2. The input file contains no labels
    3. The input file contains at maximum one instruction per line
    4. All instructions have at maximum MAX_ARGS arguments
    5. The symbol table file is well formatted
    6. The symbol table file contains all the symbol required for translation
   If an error is reached, DO NOT EXIT the function. Keep translating the rest of
   the document, and at the end, return -1. Return 0 if no errors were encountered. */
int pass_two(FILE* inter, FILE* symtbl, FILE* output) {
    /* YOUR CODE HERE */

    fprintf(output, ".text\n");
    SymbolTable *sym = create_table_from_file(SYMBOLTBL_UNIQUE_NAME, symtbl);

    /* Counter of errors */
    int errors = 0;

    /* Since we pass this buffer to strtok(),
       the chars here will GET CLOBBERED.  */
    char buf[BUF_SIZE];

    /* Store input line number / byte offset below.
       When should each be incremented?  */
    int lineNumber = 0, byteOffset = 0;

    /* First, read the next line into a buffer. */
    char* args[MAX_ARGS];
    int num_args;

    /* Define delimiters. As IGNORE_CHARS has already been defined */
    const char* delimiters = IGNORE_CHARS;
    char *name = NULL, *token = NULL;

    /* For each line, the following issues are regarded as errors
       1. Failure on translating instrution
       */
    while(fgets(buf, sizeof(buf), inter) != NULL){

        /* First accumulate line number */
        lineNumber++;

        /* Next, use strtok() to scan for next character.
           Get instruction name */
        name = strtok(buf, delimiters);
        /* If there's nothing, go to the next line */
        if(name == NULL) continue;

        /* Parse for instruction arguments.
           You should use strtok() to tokenize the rest of the line.
           Extra arguments should be filtered out in pass_one(),
           so you don't need to worry about that here.  */
        num_args = 0;
        while((token = strtok(NULL, delimiters)) != NULL){
            /* accumulate num_args
              note that after num_args accumulated, any checkup
              or operations on current argument should consider substract
              one from the current num_args */
            num_args++;
            args[num_args - 1] = token;
        }

        /* Use translate_inst() to translate the instruction and
           write to output file. */
        if(translate_inst(output, name, args, num_args, byteOffset, sym) == -1){
            /* If an error occurs,
               the instruction will not be written and you should call
               raise_instruction_error().  */
            raise_instruction_error(lineNumber, name, args, num_args);
            errors++;
            continue;
        }


        /* 1. accumulate byte offset
           2. byte offset is only accumulated when the current line
           contains a valid instruction. ALL THINGS SHOULD BE FINE HERE.
           3. 32-bit system, 4 bytes for a single instruction
           */
        byteOffset += 4;
    }
    
    fprintf(output, "\n.symbol\n");
    write_table(sym, output);

    return errors > 0 ? -1 : 0;
}

/*******************************
 * Do Not Modify Code Below
 *******************************/

static int open_files_pass_one(FILE** input, FILE** inter, FILE** symtbl,
    const char* input_name, const char* inter_name, const char* symtbl_name) {

    *input = fopen(input_name, "r");
    if (!*input) {
        write_to_log("Error: unable to open input file: %s\n", input_name);
        return -1;
    }
    *inter = fopen(inter_name, "w");
    if (!*inter) {
        write_to_log("Error: unable to open intermediate file: %s\n", inter_name);
        fclose(*input);
        return -1;
    }
    *symtbl = fopen(symtbl_name, "w");
    if (!*symtbl) {
        write_to_log("Error: unable to open symbol table file: %s\n", symtbl_name);
        fclose(*input);
        fclose(*inter);
        return -1;
    }
    return 0;
}

static int open_files_pass_two(FILE** inter, FILE** symtbl, FILE** output,
    const char* inter_name, const char* symtbl_name, const char* output_name) {

    *inter = fopen(inter_name, "r");
    if (!*inter) {
        write_to_log("Error: unable to open intermediate file: %s\n", inter_name);
        return -1;
    }
    *symtbl = fopen(symtbl_name, "r");
    if (!*symtbl) {
        write_to_log("Error: unable to open symbol table file: %s\n", inter_name);
        fclose(*inter);
        return -1;
    }
    *output = fopen(output_name, "w");
    if (!*output) {
        write_to_log("Error: unable to open output file: %s\n", output_name);
        fclose(*inter);
        fclose(*symtbl);
        return -1;
    }
    return 0;
}

static void close_files(FILE* file1, FILE* file2, FILE* file3) {
    fclose(file1);
    fclose(file2);
    fclose(file3);
}

/* Runs the two-pass assembler. Most of the actual work is done in pass_one()
   and pass_two().
 */
int assemble(const char *in, const char *out, const char *int_, const char *sym) {
    FILE *input, *inter, *symtbl, *output;
    int err = 0;

    if (in) {
        printf("Running pass one: %s -> %s, %s\n", in, int_, sym);
        if (open_files_pass_one(&input, &inter, &symtbl, in, int_, sym) != 0) {
            exit(1);
        }

        if (pass_one(input, inter, symtbl) != 0) {
            err = 1;
        }
        close_files(input, inter, symtbl);
    }

    if (out) {
        printf("Running pass two: %s, %s -> %s\n", int_, sym, out);
        if (open_files_pass_two(&inter, &symtbl, &output, int_, sym, out) != 0) {
            exit(1);
        }

        if (pass_two(inter, symtbl, output) != 0) {
            err = 1;
        }

        close_files(inter, symtbl, output);
    }

    return err;
}

static void print_usage_and_exit() {
    printf("Usage:\n");
    printf("  Runs both passes: assembler <input file> <intermediate file> <symbol table file> <output file>\n");
    printf("  Run pass #1:      assembler -p1 <input file> <intermediate file> <symbol table file>\n");
    printf("  Run pass #2:      assembler -p2 <intermediate file> <symbol table> <output file>\n");
    printf("Append -log <file name> after any option to save log files to a text file.\n");
    exit(0);
}

int main(int argc, char **argv) {
    int mode = 0;
    char *input, *inter, *output, *symtbl;
    int err;

    if (argc != 5 && argc != 7) {
        print_usage_and_exit();
    }

    if (strcmp(argv[1], "-p1") == 0) {
        mode = 1;
    } else if (strcmp(argv[1], "-p2") == 0) {
        mode = 2;
    }

    if (mode == 1) {
        input = argv[2];
        inter = argv[3];
        symtbl = argv[4];
        output = NULL;
    } else if (mode == 2) {
        input = NULL;
        inter = argv[2];
        symtbl = argv[3];
        output = argv[4];
    } else {
        input = argv[1];
        inter = argv[2];
        symtbl = argv[3];
        output = argv[4];
    }

    if (argc == 7) {
        if (strcmp(argv[5], "-log") == 0) {
            set_log_file(argv[6]);
        } else {
            print_usage_and_exit();
        }
    }

    err = assemble(input, output, inter, symtbl);

    if (err) {
        write_to_log("One or more errors encountered during assembly operation.\n");
    } else {
        write_to_log("Assembly operation completed successfully!\n");
    }

    if (is_log_file_set()) {
        printf("Results saved to %s\n", argv[5]);
    }

    return err;
}
