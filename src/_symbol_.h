#ifndef BTCHWRK_SYMBOL_PRVT_H
#define BTCHWRK_SYMBOL_PRVT_H

#include "_hashmap_.h"
#include "dynamicarray.h"
#include "symbol.h"


typedef struct
StringPage
{
	struct StringPage *next;
	size_t             used;
	char               data[SYMBOL_PAGE_SIZE];
}
StringPage;

struct
SymbolTable
{
	Hash         forward; /* string -> Symbol */
	const char **reverse; /* Symbol -> string (DynamicArray of const char*) */
	StringPage  *pages;   /* paged string pool */
	uint32_t     count;
};


#endif /* BTCHWRK_SYMBOL_PRVT_H */
