/* This project is based on the MIPS Assembler of CS61C in UC Berkeley.
   The framework of this project is been modified to be suitable for RISC-V
   in CS110 course in ShanghaiTech University by Zhijie Yang in March 2019.
   Updated by Chibin Zhang and Zhe Ye in March 2021.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "utils.h"
#include "tables.h"

const int SYMBOLTBL_NON_UNIQUE = 0;
const int SYMBOLTBL_UNIQUE_NAME = 1;

/*******************************
 * Helper Functions
 *******************************/

void allocation_failed() {
    write_to_log("Error: allocation failed\n");
    exit(1);
}

void addr_alignment_incorrect() {
    write_to_log("Error: address is not a multiple of 4.\n");
}

void name_already_exists(const char* name) {
    write_to_log("Error: name '%s' already exists in table.\n", name);
}

void write_sym(FILE* output, uint32_t addr, const char* name) {
    fprintf(output, "%u\t%s\n", addr, name);
}

/*******************************
 * Symbol Table Functions
 *******************************/

/* Creates a new SymbolTable containg 0 elements and returns a pointer to that
   table. Multiple SymbolTables may exist at the same time.
   If memory allocation fails, you should call allocation_failed().
   Mode will be either SYMBOLTBL_NON_UNIQUE or SYMBOLTBL_UNIQUE_NAME. You will need
   to store this value for use during add_to_table().
 */
SymbolTable* create_table(int mode) {
    /* YOUR CODE HERE */
    SymbolTable* symtb = (SymbolTable*)malloc(sizeof(SymbolTable));
    if (symtb == NULL) {
      allocation_failed();
    }
    /** init capacity */
    symtb -> cap = INCREMENT_OF_CAP;
    /** alloc table memory */
    symtb -> tbl = (Symbol*)malloc(sizeof(Symbol) * (symtb -> cap));
    if (symtb -> tbl == NULL) {
      allocation_failed();
    }
    /** init length */
    symtb -> len = 0;
    /** init mode */
    symtb -> mode = mode;

    return symtb;
}

/* Frees the given SymbolTable and all associated memory. */
void free_table(SymbolTable* table) {
    /* YOUR CODE HERE */
    uint32_t i;
    Symbol* temp;

    /** if table is invalid */
    if (table == NULL) {
        return;
    }

    /** free symbol names */
    for (i = 0; i < table -> len; i++) {
        temp = &(table -> tbl)[i];
        free(temp -> name);
    }

    /** free table memory on heap */
    free(table -> tbl);
    free(table);
}

/* Adds a new symbol and its address to the SymbolTable pointed to by TABLE.
   1. ADDR is given as the byte offset from the first instruction.
   2. The SymbolTable must be able to resize itself as more elements are added.

   3. Note that NAME may point to a temporary array, so it is not safe to simply
   store the NAME pointer. You must store a copy of the given string.

   4. If ADDR is not word-aligned, you should call addr_alignment_incorrect()
   and return -1.

   5. If the table's mode is SYMTBL_UNIQUE_NAME and NAME already exists
   in the table, you should call name_already_exists() and return -1.

   6.If memory allocation fails, you should call allocation_failed().

   Otherwise, you should store the symbol name and address and return 0.
 */
int add_to_table(SymbolTable* table, const char* name, uint32_t addr) {
    /* YOUR CODE HERE */
    uint32_t i;
    Symbol* temp, *sym, *pre_tbl;

    if (table == NULL) {
        return -1;
    }

    /** check word-aligned */
    if (addr % 4 != 0) {
        addr_alignment_incorrect();
        return -1;
    }

    /** if the mode requires uniqueness */
    if (table -> mode == SYMBOLTBL_UNIQUE_NAME) {
        for (i = 0; i < table -> len; i++) {
            temp = &(table -> tbl)[i];
            /** if existing */
            if (strcmp(name, temp -> name) == 0) {
                /** throw error */
                name_already_exists(name);
                return -1;
            }
        }
    }

    /** get the added element */
    sym = &(table -> tbl)[table -> len];
    /** copy name */
    sym -> name = (char*)malloc(strlen(name) + 1);
    if (sym -> name == NULL) {
      allocation_failed();
    }
    strcpy(sym -> name, name);
    /** set address */
    sym -> addr = addr;

    (table -> len)++;

    if (table -> len >= table -> cap) {
        /** increase capacity */
        table -> cap += INCREMENT_OF_CAP;

        /** re-alloc memory */
        temp = (Symbol*)malloc(sizeof(Symbol) * (table -> cap));
        if (temp == NULL) {
            allocation_failed();
        }
        /** copy memory */
        memcpy(temp, table -> tbl, sizeof(Symbol) * (table -> len));
        pre_tbl = table -> tbl;
        table -> tbl = temp;
        /** free old table */
        free(pre_tbl);
    }

    return 0;
}

/* Returns the address (byte offset) of the given symbol. If a symbol with name
   NAME is not present in TABLE, return -1.
 */
int64_t get_addr_for_symbol(SymbolTable* table, const char* name) {
    /* YOUR CODE HERE */
    uint32_t i;
    Symbol* temp;

    /** validate table */
    if (table == NULL) {
        return -1;
    }

    /** search for symbol */
    for (i = 0; i < table -> len; i++) {
        temp = &(table -> tbl)[i];
        /** compare current element and target element */
        if (strcmp(name, temp -> name) == 0) {
            return temp -> addr;
        }
    }

    return -1;
}

/* Writes the SymbolTable TABLE to OUTPUT. You should use write_sym() to
   perform the write. Do not print any additional whitespace or characters.
 */
void write_table(SymbolTable* table, FILE* output) {
    /* YOUR CODE HERE */
    uint32_t i;
    Symbol* temp;

    /** validation of table and output */
    if (table == NULL || output == NULL) {
        return;
    }

    /** write symbols one by one */
    for (i = 0; i < table -> len; i++) {
        temp = &(table -> tbl)[i];
        write_sym(output, temp -> addr, temp -> name);
    }
}

/* Read the symbol table file written by the `write_table` function, and
   restore the symbol table.
 */
SymbolTable* create_table_from_file(int mode, FILE* file) {
    SymbolTable* symtbl = create_table(mode);
    char buf[1024];
    while (fgets(buf, sizeof(buf), file)) {
        char *name, *addr_str, *end_ptr;
        uint32_t addr;
        addr_str = strtok(buf, "\t");
        addr = strtol(addr_str, &end_ptr, 0);
        name = strtok(NULL, "\n");
        add_to_table(symtbl, name, addr);
    }
    return symtbl;
}