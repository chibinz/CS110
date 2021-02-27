
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tables.h"
#include "utils.h"

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

void name_already_exists(const char *name) {
    write_to_log("Error: name '%s' already exists in table.\n", name);
}

void write_sym(FILE *output, uint32_t addr, const char *name) {
    fprintf(output, "%u\t%s\n", addr, name);
}

/*******************************
 * Symbol Table Functions
 *******************************/

/* Creates a new SymbolTable containg 0 elements and returns a pointer to that
   table. Multiple SymbolTables may exist at the same time.
   If memory allocation fails, you should call allocation_failed().
   Mode will be either SYMBOLTBL_NON_UNIQUE or SYMBOLTBL_UNIQUE_NAME. You will
   need to store this value for use during add_to_table().
 */
SymbolTable *create_table(int mode) {
    /* YOUR CODE HERE */
    SymbolTable *symbTable = malloc(sizeof(SymbolTable));
    if (symbTable == NULL) {
        allocation_failed();
    }
    symbTable->tbl = malloc(sizeof(Symbol));
    if (symbTable->tbl == NULL) {
        free(symbTable);
        allocation_failed();
    }
    symbTable->cap = 1;
    symbTable->len = 0;
    symbTable->mode = mode;
    return symbTable;
}

/* Frees the given SymbolTable and all associated memory. */
void free_table(SymbolTable *table) {
    /* YOUR CODE HERE */
    Symbol *tbl = table->tbl;
    int i;
    for (i = 0; i < table->len; i++) {
        free(tbl[i].name);
    }
    free(tbl);
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
int add_to_table(SymbolTable *table, const char *name, uint32_t addr) {
    /* YOUR CODE HERE */
    Symbol *temp = malloc(
        sizeof(Symbol)); /*Declares the variable pointers to be inserted*/
    temp->name = malloc(sizeof(name) * 4 + 1);
    int i;
    for (i = 0; i < table->len;
         i++) /*Iter over the table to see if there exists the same name*/
    {
        if (strcmp(table->tbl[i].name, name) == 0 && table->mode) {
            name_already_exists(
                name); /*Calls name_already_exists and returns -1 if true.*/
            free(temp->name);
            free(temp); /*Frees the memory on the heap before it returns*/
            return -1;
        }
    }
    if (addr % 4 != 0) /*Case not aligned*/
    {
        addr_alignment_incorrect(); /*Calls addr_alignment_incorrect and returns
                                       -1 if true*/
        free(temp->name);
        free(temp); /*Frees the memory on the heap before it returns*/
        return -1;
    }
    if (table->len >= table->cap) /*If tbl is not long enough, increase it.*/
    {
        table->tbl =
            (Symbol *)realloc(table->tbl, (table->cap * 2 * sizeof(Symbol)));
        table->cap *= 2;
        strcpy(temp->name, name); /*Copy the name to temp->name in a safe way*/
        temp->addr = addr;        /*Adds values*/
        table->tbl[i] = *temp;    /*Adds the value of temp into tbl[i]*/
        free(temp);               /*Frees the now useless temp*/
        table->len++;             /*Increase its length counter*/
        return 0;
    } else /*Case long enough*/
    {
        strcpy(temp->name, name); /*Do the same things except for increasing its
                                     capacity as above*/
        temp->addr = addr;
        table->tbl[i] = *temp;
        free(temp);
        table->len++;
        return 0;
    }

    return -1;
}

/* Returns the address (byte offset) of the given symbol. If a symbol with name
   NAME is not present in TABLE, return -1.
 */
int64_t get_addr_for_symbol(SymbolTable *table, const char *name) {
    /* YOUR CODE HERE */
    int i;
    for (i = 0; i < table->len; i++) { /*Iter through the whole tbl*/
        if (strcmp(table->tbl[i].name, name) ==
            0) /*If the name we are looking for exists*/
        {
            return table->tbl[i].addr; /*Returns its addr*/
        }
    }
    return -1; /*Returns -1 if there does not exists the name we are looking
                  for.*/
}

/* Writes the SymbolTable TABLE to OUTPUT. You should use write_sym() to
   perform the write. Do not print any additional whitespace or characters.
 */
void write_table(SymbolTable *table, FILE *output) {
    /* YOUR CODE HERE */
    int i;
    for (i = 0; i < (*table).len; i++) { /*Iter through the whole tbl*/
        write_sym(
            output, table->tbl[i].addr,
            table->tbl[i]
                .name); /*Call write_sym and puts the parameters into it.*/
    }
}
