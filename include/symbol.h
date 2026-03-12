#ifndef SYMBOL_H
#define SYMBOL_H

#include "common.h"


#define SYMBOL_INVALID   UINT32_MAX
#define SYMBOL_PAGE_SIZE 4096


typedef struct SymbolTable SymbolTable;
typedef uint32_t           Symbol;

/*
	CONSTRUCTOR / DESTRUCTOR
*/
SymbolTable *SymbolTable_new(void);
void         SymbolTable_free(SymbolTable *symbol_table);

/*
	PUBLIC METHODS
*/
Symbol       SymbolTable_intern(SymbolTable *self, const char *str);
const  char *SymbolTable_lookup(SymbolTable *self, Symbol      id);

#endif /* SYMBOL_H */
