#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "symbol.h"


static void
test_basic(void)
{
    printf("--- test_basic ---\n");

    SymbolTable *st = SymbolTable_new();
    assert(st);

    Symbol s_foo = SymbolTable_intern(st, "foo");
    Symbol s_bar = SymbolTable_intern(st, "bar");
    Symbol s_baz = SymbolTable_intern(st, "baz");

    assert(s_foo != SYMBOL_INVALID);
    assert(s_bar != SYMBOL_INVALID);
    assert(s_baz != SYMBOL_INVALID);
    printf("  intern: PASS\n");

    /* Equal strings must produce the same symbol */
    assert(SymbolTable_intern(st, "foo") == s_foo);
    assert(SymbolTable_intern(st, "bar") == s_bar);
    assert(SymbolTable_intern(st, "baz") == s_baz);
    printf("  dedup: PASS\n");

    /* Different strings must produce different symbols */
    assert(s_foo != s_bar);
    assert(s_foo != s_baz);
    assert(s_bar != s_baz);
    printf("  uniqueness: PASS\n");

    /* Reverse lookup */
    assert(strcmp(SymbolTable_lookup(st, s_foo), "foo") == 0);
    assert(strcmp(SymbolTable_lookup(st, s_bar), "bar") == 0);
    assert(strcmp(SymbolTable_lookup(st, s_baz), "baz") == 0);
    printf("  lookup: PASS\n");

    /* Invalid id returns NULL */
    assert(SymbolTable_lookup(st, SYMBOL_INVALID) == NULL);
    assert(SymbolTable_lookup(st, 9999)           == NULL);
    printf("  invalid lookup: PASS\n");

    SymbolTable_free(st);
    printf("  free: PASS\n");
}


static void
test_pointer_stability(void)
{
    printf("--- test_pointer_stability ---\n");

    SymbolTable *st = SymbolTable_new();
    assert(st);

    /* Intern from a stack buffer — the returned pointer must outlive it */
    Symbol s;
    {
        char buf[64];
        strcpy(buf, "ephemeral");
        s = SymbolTable_intern(st, buf);
    }

    /* buf is gone but the interned string must still be valid */
    const char *interned = SymbolTable_lookup(st, s);
    assert(interned != NULL);
    assert(strcmp(interned, "ephemeral") == 0);
    printf("  outlives source buffer: PASS\n");

    /* Re-interning the same content returns the same pointer */
    Symbol s2 = SymbolTable_intern(st, "ephemeral");
    assert(s2 == s);
    assert(SymbolTable_lookup(st, s2) == interned);
    printf("  stable pointer on re-intern: PASS\n");

    SymbolTable_free(st);
    printf("  free: PASS\n");
}


static void
test_symbol_comparison(void)
{
    printf("--- test_symbol_comparison ---\n");

    SymbolTable *st = SymbolTable_new();
    assert(st);

    /* The whole point of symbols: comparison is just == on integers,
       no strcmp needed */
    Symbol s_if     = SymbolTable_intern(st, "if");
    Symbol s_else   = SymbolTable_intern(st, "else");
    Symbol s_while  = SymbolTable_intern(st, "while");
    Symbol s_if2    = SymbolTable_intern(st, "if");

    assert(s_if == s_if2);
    assert(s_if != s_else);
    assert(s_if != s_while);
    printf("  integer comparison: PASS\n");

    SymbolTable_free(st);
    printf("  free: PASS\n");
}


static void
test_many_symbols(void)
{
    printf("--- test_many_symbols ---\n");

    SymbolTable *st = SymbolTable_new();
    assert(st);

    /* Intern enough symbols to force the HashMap to grow and rehash */
    #define COUNT 256
    char    keys[COUNT][16];
    Symbol  ids[COUNT];

    for (int i = 0; i < COUNT; i++) {
        snprintf(keys[i], sizeof(keys[i]), "sym_%d", i);
        ids[i] = SymbolTable_intern(st, keys[i]);
        assert(ids[i] != SYMBOL_INVALID);
    }
    printf("  intern x%d: PASS\n", COUNT);

    /* All symbols must still be found correctly after any rehash */
    for (int i = 0; i < COUNT; i++) {
        assert(SymbolTable_intern(st, keys[i]) == ids[i]);
        assert(strcmp(SymbolTable_lookup(st, ids[i]), keys[i]) == 0);
    }
    printf("  lookup x%d after growth: PASS\n", COUNT);

    /* All ids must be unique */
    for (int i = 0; i < COUNT; i++)
        for (int j = i + 1; j < COUNT; j++)
            assert(ids[i] != ids[j]);
    printf("  uniqueness x%d: PASS\n", COUNT);

    SymbolTable_free(st);
    printf("  free: PASS\n");
    #undef COUNT
}


static void
test_multiple_tables(void)
{
    printf("--- test_multiple_tables ---\n");

    /* Two independent tables — same string may get different ids in each */
    SymbolTable *st1 = SymbolTable_new();
    SymbolTable *st2 = SymbolTable_new();
    assert(st1 && st2);

    Symbol a1 = SymbolTable_intern(st1, "alpha");
    Symbol b1 = SymbolTable_intern(st1, "beta");

    /* Intern in a different order in st2 */
    Symbol b2 = SymbolTable_intern(st2, "beta");
    Symbol a2 = SymbolTable_intern(st2, "alpha");

    /* Within each table, equal strings give equal symbols */
    assert(SymbolTable_intern(st1, "alpha") == a1);
    assert(SymbolTable_intern(st2, "alpha") == a2);
    printf("  per-table dedup: PASS\n");

    /* Tables are independent — ids may differ */
    assert(strcmp(SymbolTable_lookup(st1, a1), "alpha") == 0);
    assert(strcmp(SymbolTable_lookup(st2, a2), "alpha") == 0);
    assert(strcmp(SymbolTable_lookup(st1, b1), "beta")  == 0);
    assert(strcmp(SymbolTable_lookup(st2, b2), "beta")  == 0);
    printf("  table independence: PASS\n");

    SymbolTable_free(st1);
    SymbolTable_free(st2);
    printf("  free: PASS\n");
}


int
main(void)
{
    test_basic();
    test_pointer_stability();
    test_symbol_comparison();
    test_many_symbols();
    test_multiple_tables();

    printf("\nAll tests passed.\n");
    return 0;
}
