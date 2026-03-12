#include <stdlib.h>
#include <string.h>

#include "_symbol_.h"
#include "hashfuncs.h"


/***********************
    Internal helpers
***********************/
static size_t
symbol_str_hash(const void *data, size_t size)
{
    (void)size;
    /* data points to a char* — dereference to get the string */
    return hash_fnv1a_str(*(const char **)data);
}

static int
symbol_str_cmp(const void *a, const void *b, size_t size)
{
    (void)size;
    /* a and b are char* pointers — dereference to compare strings */
    return strcmp(*(const char **)a, *(const char **)b);
}

/*  Intern a string into the pool, returning a stable pointer.
    Strings are bump-allocated into pages; a new page is chained
    when the current one is full.
*/
static const char *
pool_intern(SymbolTable *self, const char *str)
{
    size_t len = strlen(str) + 1;   /* include null terminator */

    /* Walk pages looking for one with enough room */
    StringPage *page = self->pages;
    while (page) {
        if (SYMBOL_PAGE_SIZE - page->used >= len)
            break;
        page = page->next;
    }

    /* No page with room — allocate a new one */
    if (!page) {
        page = calloc(1, sizeof(StringPage));
        if (!page)
            return NULL;
        page->next  = self->pages;
        self->pages = page;
    }

    char *dst = page->data + page->used;
    memcpy(dst, str, len);
    page->used += len;
    return dst;
}


/***********************
    Constructor / Destructor
***********************/
SymbolTable *
SymbolTable_new(void)
{
    SymbolTable *self = calloc(1, sizeof(SymbolTable));
    if (!self)
        return NULL;

    /* Key is a fixed-size string buffer — we use pointer-sized keys here:
       the key stored in the HashMap is the interned char* itself, so
       key_size = sizeof(char*) and we hash/compare via the pointed-to string. */
    if (!HashMap_init(&self->forward, sizeof(char *), sizeof(Symbol),
                      symbol_str_hash, symbol_str_cmp)) {
        DynamicArray_free(self->reverse);
        free(self);
        return NULL;
    }

    self->reverse = DynamicArray(const char *, 64);
    if (!self->reverse) {
        HashMap_destroy(&self->forward);
        free(self);
        return NULL;
    }

    return self;
}

void
SymbolTable_free(SymbolTable *self)
{
    HashMap_destroy(&self->forward);
    DynamicArray_free(self->reverse);

    StringPage *page = self->pages;
    while (page) {
        StringPage *next = page->next;
        free(page);
        page = next;
    }

    free(self);
}


/***********************
    Public methods
***********************/
Symbol
SymbolTable_intern(SymbolTable *self, const char *str)
{
    /* Fast path — already interned; key is a char*, hash/cmp dereference it */
    void *existing = HashMap_get(&self->forward, &str);
    if (existing)
        return *(Symbol *)existing;

    /* Copy string into the pool for stable storage */
    const char *interned = pool_intern(self, str);
    if (!interned)
        return SYMBOL_INVALID;

    /* Assign the next ID */
    Symbol id = self->count++;

    /* Forward: interned char* (as key) -> id
       Storing the interned pointer as the key means future lookups via
       any char* with equal content will hash/cmp correctly. */
    HashMap_put(&self->forward, &interned, &id);

    /* Reverse: id -> interned pointer (id is the index) */
    DynamicArray_replace((void **)&self->reverse, id, &interned, 1);

    return id;
}

const char *
SymbolTable_lookup(SymbolTable *self, Symbol id)
{
    if (id >= self->count)
        return NULL;
    return self->reverse[id];
}
